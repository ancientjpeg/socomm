/**
 * @file broadcast_handler.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_BROADCAST_HANDLER_H_
#define SOCOMM_BROADCAST_HANDLER_H_

#include "str.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct socomm_broadcast_handler_t socomm_broadcast_handler;

socomm_broadcast_handler                 *socomm_broadcast_handler_create();

void socomm_broadcast_handler_destroy(socomm_broadcast_handler **bh);

void socomm_broadcast_handler_post_sstr(socomm_broadcast_handler *bh,
                                        socomm_string            *str);

void socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                   void                     *data,
                                   size_t                    size);

/**
 * @brief Poll this handler for messages from other broadcasters. Outputs
 * a single message into
 *
 * @param bh
 * @param data Pointer to data to write the message into.
 * @param size
 * @return
 */
int  socomm_broadcast_handler_poll(socomm_broadcast_handler *bh,
                                   socomm_string           **str_ptr);

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh);

#endif
