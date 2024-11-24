/**
 * @file broadcast_handler.c
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

void socomm_broadcast_handler_post_sstr(socomm_broadcast_handler *bh,
                                        socomm_buffer            *str)
{
  socomm_broadcast_handler_post(bh, (void *)socomm_buffer_data(str),
                                socomm_buffer_size(str));
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

int socomm_broadcast_handler_poll(socomm_broadcast_handler *bh,
                                  socomm_buffer           **buf_ptr)
{

  /* https://gist.github.com/Mystfit/6c015257b637ae31bcb63130da67627c */
  char read_buf[1024];
  memset(read_buf, 0, 1024);
  zmq_msg_t recv_msg;
  zmq_msg_init(&recv_msg);

  int    recv_code = zmq_msg_recv(&recv_msg, bh->dish_socket_, ZMQ_DONTWAIT);

  void  *data      = zmq_msg_data(&recv_msg);
  size_t size      = zmq_msg_size(&recv_msg);

  socomm_buffer_destroy(buf_ptr);
  *buf_ptr = socomm_buffer_create_data(data, size);

  zmq_msg_close(&recv_msg);

  if (recv_code == -1) {
    if (errno == EAGAIN) {
      return EAGAIN;
    }
    return -1;
  }

  return true;
}

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh)
{
  zmq_close(bh->radio_socket_);
  zmq_close(bh->dish_socket_);
}