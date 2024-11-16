/**
 * @file broadcast_handler.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "broadcast_handler.h"
#include "socomm/helpers.h"
#include <array>
#include <cassert>
#include <iostream>

namespace socomm {

broadcast_handler::broadcast_handler()
{
  //  Connecting using a Multicast address
  radio_socket_ = zmq_socket(helpers::get_global_zmq_ctx(), ZMQ_RADIO);
  int rc        = zmq_connect(radio_socket_, "udp://239.0.0.1:9325");
  helpers::handle_errno(rc);

  dish_socket_ = zmq_socket(helpers::get_global_zmq_ctx(), ZMQ_DISH);
  rc           = zmq_connect(dish_socket_, "udp://239.0.0.1:9325");
  helpers::handle_errno(rc);
}

broadcast_handler::~broadcast_handler()
{
  zmq_close(radio_socket_);
  zmq_close(dish_socket_);
}

void broadcast_handler::post(std::string msg)
{
  int rc = zmq_send(radio_socket_, msg.data(), msg.size(), 0);

  helpers::handle_errno(rc);
}

void broadcast_handler::poll()
{
  std::array<char, 1024> read_buf = {};
  int rc = zmq_recv(dish_socket_, read_buf.data(), read_buf.size(), 0);

  if (rc == -1 && errno == EAGAIN) {
    return;
  }

  helpers::handle_errno(rc);

  std::cout << std::string_view(read_buf.data(), read_buf.size()) << std::endl;
}

} // namespace socomm
