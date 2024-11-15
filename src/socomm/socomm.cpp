/**
 * @file socomm.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-14
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/socomm.h"
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <vector>

namespace socomm {

inline void handle_status(int status)
{
  if (status != 0) {
    printf("ERROR: %s\n", zmq_strerror(status));
    exit(status);
  }
}

inline void handle_errno(int status)
{
  if (status == -1) {
    printf("ERROR: %s\n", strerror(errno));
    exit(errno);
  }
}

node::node()
{
  printf("Begin\n");
  context_ = zmq_ctx_new();
  printf("Got Context\n");

  int status;

  socket_ = zmq_socket(context_, ZMQ_REP);
  status  = zmq_bind(socket_, "tcp://*:5555");
  handle_status(status);

  void *client_socket = zmq_socket(context_, ZMQ_REQ);
  status              = zmq_connect(client_socket, "tcp://localhost:5555");
  handle_status(status);

  std::array<char, 128> buff;

  for (int i = 0; i < 10000; ++i) {
    printf("Client send\n");
    status = zmq_send(client_socket, "Please", 6, 0);
    handle_errno(status);

    printf("Socket receive\n");
    buff.fill(0);
    status = zmq_recv(socket_, buff.data(), buff.size(), 0);
    handle_errno(status);

    printf("Server socket receieved request: \"%s\"\n", buff.data());

    printf("Socket send\n");
    status = zmq_send(socket_, "Thank you", 9, 0);
    handle_errno(status);

    printf("Client receive\n");
    buff.fill(0);
    status = zmq_recv(client_socket, buff.data(), buff.size(), 0);
    handle_errno(status);

    printf("Client socket receieved request: \"%s\"\n", buff.data());
  }

  zmq_close(client_socket);
}

node::~node()
{
  zmq_close(socket_);
  zmq_ctx_destroy(context_);
}

} // namespace socomm
