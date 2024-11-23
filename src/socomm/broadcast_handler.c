/**
 * @file broadcast_handler.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "broadcast_handler.h"
#include "helpers.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

typedef struct socomm_broadcast_handler_t {
  void *radio_socket_;
  void *dish_socket_;
} socomm_broadcast_handler;

const char               *addr  = "udp://239.0.0.1:9325";
const char               *gname = "radio_group";

socomm_broadcast_handler *socomm_broadcast_handler_create()
{
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

  rc = zmq_join(bh->dish_socket_, gname);
  socomm_handle_errno(rc);

  return bh;
}

void socomm_broadcast_handler_destroy(socomm_broadcast_handler **bh)
{
  socomm_broadcast_handler_disconnect(*bh);
  *bh = NULL;
}

void socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                   void                     *data,
                                   size_t                    size)
{

  void *msg_data = malloc(size);
  memcpy(msg_data, data, size);
  zmq_msg_t out_msg;
  zmq_msg_init_data(&out_msg, msg_data, size, socomm_compatible_free, NULL);
  zmq_msg_set_group(&out_msg, gname);

  int rc = zmq_sendmsg(bh->radio_socket_, &out_msg, 0);
  socomm_handle_errno(rc);
}

bool socomm_broadcast_handler_poll(socomm_broadcast_handler *bh)
{
  /* https://gist.github.com/Mystfit/6c015257b637ae31bcb63130da67627c */
  char read_buf[1024];
  memset(read_buf, 0, 1024);
  zmq_msg_t recv_msg;
  zmq_msg_init(&recv_msg);

  int    rc   = zmq_recvmsg(bh->dish_socket_, &recv_msg, ZMQ_DONTWAIT);

  void  *data = zmq_msg_data(&recv_msg);
  size_t size = zmq_msg_size(&recv_msg);
  if (size) {
    printf("%s\n", (char *)data);
  }

  zmq_msg_close(&recv_msg);

  if (rc == -1 && errno == EAGAIN) {
    return false;
  }

  socomm_handle_errno(rc);

  return true;
}

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh)
{
  zmq_close(bh->radio_socket_);
  zmq_close(bh->dish_socket_);
}
