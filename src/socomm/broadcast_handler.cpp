/**
 * @file broadcast_handler.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "broadcast_handler.h"
#include "socomm/helpers.h"
#include "socomm/uuid.h"
#include <array>
#include <cassert>
#include <iostream>

namespace socomm {

const char *addr  = "udp://239.0.0.1:9325";
const char *gname = "radio_group";

broadcast_handler::broadcast_handler()
{
  //  Connecting using a Multicast address
  radio_socket_ = zmq_socket(helpers::get_global_zmq_ctx(), ZMQ_RADIO);
  int rc        = zmq_connect(radio_socket_, addr);
  if (rc == -1) {
    disconnect();
  }
  helpers::handle_errno(rc);

  dish_socket_ = zmq_socket(helpers::get_global_zmq_ctx(), ZMQ_DISH);
  rc           = zmq_bind(dish_socket_, addr);
  if (rc == -1) {
    disconnect();
  }
  helpers::handle_errno(rc);

  rc = zmq_join(dish_socket_, gname);
  helpers::handle_errno(rc);
}

broadcast_handler::~broadcast_handler()
{
  disconnect();
}

void broadcast_handler::post(void *data, size_t size)
{

  void *msg_data = malloc(size);
  std::memcpy(msg_data, data, size);
  zmq_msg_t out_msg;
  zmq_msg_init_data(&out_msg, msg_data, size, helpers::compatible_free,
                    nullptr);
  zmq_msg_set_group(&out_msg, gname);

  int rc = zmq_sendmsg(radio_socket_, &out_msg, 0);
  helpers::handle_errno(rc);
}

bool broadcast_handler::poll()
{
  /* https://gist.github.com/Mystfit/6c015257b637ae31bcb63130da67627c */
  std::array<char, 1024> read_buf = {};
  zmq_msg_t              recv_msg;
  zmq_msg_init(&recv_msg);

  int    rc   = zmq_recvmsg(dish_socket_, &recv_msg, ZMQ_DONTWAIT);

  void  *data = zmq_msg_data(&recv_msg);
  size_t size = zmq_msg_size(&recv_msg);
  if (size) {
    std::cout << std::string_view((const char *)data, size) << std::endl;
  }

  zmq_msg_close(&recv_msg);

  if (rc == -1 && errno == EAGAIN) {
    return false;
  }

  helpers::handle_errno(rc);

  return true;
}

void broadcast_handler::disconnect()
{
  zmq_close(radio_socket_);
  zmq_close(dish_socket_);
}

} // namespace socomm
