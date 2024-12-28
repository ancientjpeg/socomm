/**
 * @file node.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/node.h"
#include "socomm/broadcast_handler.h"
#include "uuid/uuid.h"
#include <stdlib.h>

struct node_metadata {
  uint16_t port;
};

typedef struct socomm_node_t {
  socomm_header             header;
  socomm_broadcast_handler *broadcast_handler_;
  void                     *socket_;
} socomm_node;

socomm_node *socomm_node_create(const char *group_name)
{
  socomm_node  *node = malloc(sizeof(socomm_node));

  uuid4_state_t uuid_state;
  uuid4_t       node_uuid;
  uuid4_seed(&uuid_state);
  uuid4_gen(&uuid_state, &node_uuid);

  const uint16_t port      = 9325;
  node->header             = socomm_header_init(port, node_uuid, group_name);
  node->broadcast_handler_ = socomm_broadcast_handler_create(node->header);

  return node;
}

void socomm_node_destroy(socomm_node *node)
{
  socomm_broadcast_handler_destroy(&node->broadcast_handler_);
}
