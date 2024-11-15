/**
 * @file socomm.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-14
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_SOCOMM_H_
#define SOCOMM_SOCOMM_H_

#include <zmq.h>

namespace socomm {

class node {
public:
  node();
  ~node();

private:
  void *context_ = nullptr;
  void *socket_  = nullptr;
};

} // namespace socomm

#endif
