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

socomm_broadcast_handler      *
socomm_broadcast_handler_create(const char *group_name);

void socomm_broadcast_handler_destroy(socomm_broadcast_handler **bh);

void socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                   void                     *data,
                                   size_t                    size);

/**
 * @brief Non-blocking poll that retreieves a single message from the queue.
 * @details Identical to calling `socomm_broadcast_handler_poll_blocking` with
 * `0` as the timeout argument. Return values of -1 with `errno == EAGAIN`
 * should not be considered error states but instead indicate an empty message
 * queue.
 */
int socomm_broadcast_handler_poll(socomm_broadcast_handler *bh,
                                  socomm_string           **str_ptr);

/**
 * @brief Poll this handler for messages from other broadcasters. Outputs
 * a single message into `*str_ptr`.
 *
 * @param bh
 * @param str_ptr
 * @param timeout_ms Timeout in milliseconds. If `timeout_ms <= 0`, this
 * function will be called non-blocking, i.e. the ZMQ_DONTWAIT flag will be set.
 * @return If successful, number of bytes written. Else, -1 with `errno` set
 * in accordance with `zmq_msg_recv`. If timeout is reached, will return -1
 * with `errno == EAGAIN`
 */
int  socomm_broadcast_handler_poll_blocking(socomm_broadcast_handler *bh,
                                            socomm_string           **str_ptr,
                                            int timeout_ms);

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh);

#endif
