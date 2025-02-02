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
#include <time.h>
#include <zmq.h>

typedef struct socomm_broadcast_handler_t {
  socomm_header header;
  void         *radio_socket_;
  void         *dish_socket_;
} socomm_broadcast_handler;

const char               *addr = "udp://239.0.0.1:9325";

socomm_broadcast_handler *socomm_broadcast_handler_create(socomm_header header)
{
  const char               *group_name      = (const char *)header.group_name;

  size_t                    group_name_size = strlen(group_name);

  //  Connecting using a Multicast address
  socomm_broadcast_handler *bh
      = (socomm_broadcast_handler *)malloc(sizeof(socomm_broadcast_handler));
  bh->header        = header;
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

  int set_group_rc = zmq_msg_set_group(&out_msg, bh->header.group_name);

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
  socomm_message *message = NULL;

  /* https://gist.github.com/Mystfit/6c015257b637ae31bcb63130da67627c */
  zmq_msg_t       recv_msg;
  zmq_msg_init(&recv_msg);

  const bool   async        = timeout_ms == 0;
  bool         try_again    = false;

  const double ms_per_clock = 1e03 / CLOCKS_PER_SEC;

  clock_t      start        = clock();

  do {

    try_again     = false;

    int recv_code = zmq_msg_recv(&recv_msg, bh->dish_socket_, ZMQ_DONTWAIT);

    if (recv_code == -1) {
      if (errno != EAGAIN) {
        break;
      }
      else {
        continue;
      }
    }

    message = socomm_deserialize_message(&recv_msg);

    if (message == NULL) {
      errno = EBADMSG;
      break;
    }

    const socomm_header *header = socomm_message_header(message);

    if (header == NULL) {
      errno = EBADMSG;
      break;
    }

    const bool from_self
        = memcmp(&header->uuid, &bh->header.uuid, sizeof(uuid4_t)) == 0;

    if (!from_self) {
      break;
    }

    socomm_message_destroy(&message);
    errno     = EAGAIN;
    try_again = true;

  } while ((try_again && async)
           || ((double)(clock() - start) * ms_per_clock) < timeout_ms);

  zmq_msg_close(&recv_msg);

  return message;
}

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh)
{
  zmq_close(bh->radio_socket_);
  zmq_close(bh->dish_socket_);
}
