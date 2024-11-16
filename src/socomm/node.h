/**
 * @file node.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_NODE_H_
#define SOCOMM_NODE_H_

#include "socomm/broadcast_handler.h"

namespace socomm {

class node {
public:
  node();
  ~node();

private:
  broadcast_handler broadcast_handler_;
  void             *socket_ = nullptr;
};

} // namespace socomm

#endif
