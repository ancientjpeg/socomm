/**
 * @file broadcast_handler.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_BROADCAST_HANDLER_H_
#define SOCOMM_BROADCAST_HANDLER_H_

#include <string>

namespace socomm {
class broadcast_handler {
public:
  broadcast_handler();

  ~broadcast_handler();

  /* TODO: change to span */
  void post(std::string msg);

  void poll();

private:
  void *radio_socket_ = nullptr;
  void *dish_socket_  = nullptr;
};
} // namespace socomm

#endif
