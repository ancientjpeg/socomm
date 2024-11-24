/**
 * @file header_serde.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-24
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_HEADER_SERDE_H_
#define SOCOMM_HEADER_SERDE_H_

#include "header.h"
#include <zmq.h>

/**
 * @brief Serialize a `socomm_message` into a `zmq_msg_t`.
 *
 * @return `zmq_msg_t` containing the serialized `socomm_message`. This
 * message will be created by copy via `zmq_msg_init_buffer`.
 */
zmq_msg_t socomm_serialize_message(socomm_message *msg);

/**
 * @brief Deserialize a `zmq_msg_t` into a `socomm_header`..
 * @note Will call `zmq_msg_close` on `msg`.
 */
socomm_message *socomm_deserialize_message(zmq_msg_t *msg);

#endif
