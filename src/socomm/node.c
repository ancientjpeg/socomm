/**
 * @file node.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/node.h"
#include "socomm/broadcast_handler.h"
#include <stdlib.h>
#include <string.h>

struct node_metadata {
  uint16_t port;
};

typedef struct socomm_node_t {
  socomm_broadcast_handler *broadcast_handler_;
  void                     *socket_;
} socomm_node;

socomm_node *socomm_node_create()
{
  const char  *gname       = "default_gname";
  socomm_node *node        = malloc(sizeof(socomm_node));
  node->broadcast_handler_ = socomm_broadcast_handler_create(gname);

  return node;
}

void socomm_node_destroy(socomm_node *node)
{
  socomm_broadcast_handler_destroy(&node->broadcast_handler_);
}
