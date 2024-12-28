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
 * message will be created by copy via `zmq_msg_init_buffer`. In the event
 * of a failure, will return `NULL` and set `errno`.
 */
/**
 * @brief Serialize a `socomm_message` into a `zmq_msg_t`.
 *
 * @param msg
 * @return 0 on success, or -1 with `errno` set on error.
 */
int socomm_serialize_message(socomm_message *msg, zmq_msg_t *out_msg);

/**
 * @brief Deserialize a `zmq_msg_t` into a `socomm_header`..
 * @note Will call `zmq_msg_close` on `msg`.
 * @return The deserialized message, or `NULL` if an error occurs. Error states
 * will set `errno`.
 */
socomm_message *socomm_deserialize_message(zmq_msg_t *msg);

#endif
