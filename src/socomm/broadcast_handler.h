/**
 * @file broadcast_handler.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_BROADCAST_HANDLER_H_
#define SOCOMM_BROADCAST_HANDLER_H_

#include "header.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct socomm_broadcast_handler_t socomm_broadcast_handler;

socomm_broadcast_handler *socomm_broadcast_handler_create(socomm_header header);

void            socomm_broadcast_handler_destroy(socomm_broadcast_handler **bh);

/**
 * @brief Posts a message to all broadcast handlers in the same group.
 *
 * @param bh
 * @param message The message to send.
 * @return Number of bytes posted on success or -1 on an error with `ernno` set.
 */
int             socomm_broadcast_handler_post(socomm_broadcast_handler *bh,
                                              socomm_message           *message);

/**
 * @brief Non-blocking poll that retreieves a single message from the queue.
 * @details Identical to calling `socomm_broadcast_handler_poll_blocking` with
 * `0` as the timeout argument. Return values of NULL with `errno == EAGAIN`
 * should not be considered error states but instead indicate an empty message
 * queue.
 */
socomm_message *socomm_broadcast_handler_poll(socomm_broadcast_handler *bh);

/**
 * @brief Poll this handler for messages from other broadcasters. Outputs
 * a single message into `*str_ptr`.
 * @details `errno` values on error shall be those described by `zmq_msg_rcv`,
 * with the addition of `EBADMSG` indicating a failure to parse the zmq message
 * into a socomm message.
 *
 * @param bh
 * @param str_ptr
 * @param timeout_ms Timeout in milliseconds. If `timeout_ms <= 0`, this
 * function will be called non-blocking, i.e. the ZMQ_DONTWAIT flag will be set.
 * @return `socomm_message` containing the first intercepted message in the
 * handler's queue. On error, returns `NULL`.
 */
socomm_message      *
socomm_broadcast_handler_poll_blocking(socomm_broadcast_handler *bh,
                                            int                       timeout_ms);

void socomm_broadcast_handler_disconnect(socomm_broadcast_handler *bh);

#endif
