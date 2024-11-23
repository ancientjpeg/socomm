/**
 * @file node.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_NODE_H_
#define SOCOMM_NODE_H_

typedef struct socomm_node_t socomm_node;

socomm_node                 *socomm_node_create();
void                         socomm_node_destroy(socomm_node *node);

#endif
