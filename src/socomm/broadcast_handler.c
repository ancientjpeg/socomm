/**
 * @file broadcast_handler.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "broadcast_handler.h"
#include "header.h"
#include "header_serde.h"
#include "helpers.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

typedef struct socomm_broadcast_handler_t {
  void *radio_socket_;
  void *dish_socket_;
  char  group_name[32];
} socomm_broadcast_handler;

const char *addr = "udp://239.0.0.1:9325";

socomm_broadcast_handler *
socomm_broadcast_handler_create(const char *group_name)
{
  size_t group_name_size = strlen(group_name);
  assert(group_name_size
         < sizeof(((socomm_broadcast_handler *)NULL)->group_name));

  //  Connecting using a Multicast address
  socomm_broadcast_handler *bh
      = (socomm_broadcast_handler *)malloc(sizeof(socomm_broadcast_handler));
  bh->radio_socket_ = zmq_socket(socomm_get_global_ctx(), ZMQ_RADIO);
  int rc            = zmq_connect(bh->radio_socket_, addr);
  if (rc == -1) {
    socomm_broadcast_handler_disconnect(bh);
  }
  socomm_handle_errno(rc);

  bh->dish_socket_ = zmq_socket(socomm_get_global_ctx(), ZMQ_DISH);
  rc               = zmq_bind(bh->dish_socket_, addr);
  if (rc == -1) {
    socomm_broadcast_handler_disconnect(bh);
  }
  socomm_handle_errno(rc);

  rc = zmq_join(bh->dish_socket_, group_name);
  socomm_handle_errno(rc);

  memcpy(bh->group_name, group_name, group_name_size);
  bh->group_name[group_name_size] = '\0';
  return bh;
}

void socomm_broadcast_handler_destroy(socomm_broadcast_handler **bh)
{
  socomm_broadcast_handler_disconnect(*bh);
  *bh = NULL;
}

int socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                  socomm_message           *message)
{

  zmq_msg_t out_msg;
  int       serialize_rc = socomm_serialize_message(message, &out_msg);

  if (serialize_rc != 0) {
    return serialize_rc;
  }

  int set_group_rc = zmq_msg_set_group(&out_msg, bh->group_name);

  if (set_group_rc != 0) {
    return -1;
  }

  return zmq_msg_send(&out_msg, bh->radio_socket_, 0);
}

socomm_message *socomm_broadcast_handler_poll(socomm_broadcast_handler *bh)
{
  return socomm_broadcast_handler_poll_blocking(bh, 0);
}

socomm_message *
socomm_broadcast_handler_poll_blocking(socomm_broadcast_handler *bh,
                                       int                       timeout_ms)
{
  bool timeout = timeout_ms > 0;
  int  flags   = timeout ? 0 : ZMQ_DONTWAIT;

  /* https://gist.github.com/Mystfit/6c015257b637ae31bcb63130da67627c */
  zmq_msg_t recv_msg;
  zmq_msg_init(&recv_msg);

  if (timeout) {
    zmq_setsockopt(bh->dish_socket_, ZMQ_RCVTIMEO, &timeout_ms, sizeof(int));
  }

  int             recv_code = zmq_msg_recv(&recv_msg, bh->dish_socket_, flags);
  socomm_message *message   = NULL;

  if (recv_code == -1) {
    goto cleanup;
  }

  message = socomm_deserialize_message(&recv_msg);

cleanup:
  zmq_msg_close(&recv_msg);
  if (timeout) {
    int inf_timeout = -1;
    zmq_setsockopt(bh->dish_socket_, ZMQ_RCVTIMEO, &inf_timeout, sizeof(int));
  }

  return message;
}

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh)
{
  zmq_close(bh->radio_socket_);
  zmq_close(bh->dish_socket_);
}
