/**
 * @file broadcast_handler.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "broadcast_handler.h"
#include "socomm/helpers.h"
#include <cassert>
#include <iostream>
#include <string>

namespace socomm {

broadcast_handler::broadcast_handler()
{
  //  Connecting using a Multicast address
  broadcast_socket_ = zmq_socket(helpers::get_global_zmq_ctx(), ZMQ_RADIO);
  int rc            = zmq_connect(broadcast_socket_, "udp://239.0.0.1:9325");
  helpers::handle_status(rc);
}

broadcast_handler::~broadcast_handler()
{
  zmq_close(broadcast_socket_);
}

void broadcast_handler::post(std::string msg)
{
  int rc = zmq_send(broadcast_socket_, msg.data(), msg.size(), 0);
  helpers::handle_errno(rc);
}

void broadcast_handler::poll()
{
  std::array<char, 1024> read_buf = {};
  int rc = zmq_recv(broadcast_socket_, read_buf.data(), read_buf.size(),
                    ZMQ_DONTWAIT);

  if (rc == -1 && errno == EAGAIN) {
    return;
  }
  helpers::handle_errno(rc);

  std::cout << std::string_view(read_buf.data(), read_buf.size()) << std::endl;
}

} // namespace socomm
