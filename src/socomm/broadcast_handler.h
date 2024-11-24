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

void socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                   void                     *data,
                                   size_t                    size);

/**
 * @brief Poll this handler for messages from other broadcasters. Outputs
 * a single message into
 * @note This function may return -1 with `EAGAIN` as the errno, in which case
 * there is no critical failure and you may safely continue on as it simply
 * indicates the message queue is empty.
 *
 * @param bh
 * @param data Pointer to data to write the message into.
 * @param size
 * @return If successful, number of bytes written. Else, -1 with `errno` set
 * in accordance with `zmq_msg_recv`.
 */
int  socomm_broadcast_handler_poll(socomm_broadcast_handler *bh,
                                   socomm_string           **str_ptr);

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh);

#endif
