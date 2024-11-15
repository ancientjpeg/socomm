/**
 * @file socomm.cpp
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-14
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/socomm.h"
#include <cstdlib>

namespace socomm {

node::node()
{
  context_   = zmq_ctx_new();
  void *ctx2 = zmq_ctx_new();
  printf("Got Context\n");

  socket_    = zmq_socket(context_, ZMQ_REP);
  int status = zmq_bind(socket_, "tcp://*:9350");

  if (status != 0) {
    exit(status);
  }

  void *client_socket = zmq_socket(ctx2, ZMQ_REQ);
  status              = zmq_connect(client_socket, "tcp://localhost:9350");
  if (status != 0) {
    exit(status);
  }
  printf("Connected\n");

  char buff[12];
  printf("Socket send\n");
  status = zmq_send(socket_, "Thank you", 9, 0);
  if (status != 0) {
    printf("%s\n", zmq_strerror(status));
    exit(status);
  }
  printf("Client receive\n");
  zmq_recv(socket_, buff, 12, 0);
  printf("Client socket receieved request: %s\n", buff);

  printf("Client send\n");
  status = zmq_send(client_socket, "Please", 6, 0);
  if (status != 0) {
    exit(status);
  }
  printf("Socket receive\n");
  zmq_recv(socket_, buff, 12, 0);
  printf("Server socket receieved request: %s\n", buff);

  zmq_close(client_socket);
  zmq_ctx_destroy(ctx2);
}

node::~node()
{
  zmq_close(socket_);
  zmq_ctx_destroy(context_);
}

} // namespace socomm
