/**
 * @file node.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/node.h"
#include "socomm/broadcast_handler.h"

typedef struct socomm_node_t {
  socomm_broadcast_handler *broadcast_handler_;
  void                     *socket_;
} socomm_node;
