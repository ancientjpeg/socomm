/**
 * @file header.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_HEADER_H_
#define SOCOMM_HEADER_H_

#include "uuid/uuid.h"
#include <assert.h>
#include <stddef.h>

#define SOCOMM_HEADER_VERSION 0
static_assert(SOCOMM_HEADER_VERSION <= UINT8_MAX,
              "If SOCOMM_HEADER_VERSION has exceeded the range of a uint8_t, "
              "please panic.");

static const char *const SOCOMM_MESSAGE_TYPES[] = {
    /* Signifies a UDP heartbeat. */
    "PULSE",

    /* Request to enter a group. */
    "JOIN",

    /* Request to leave a group. */
    "LEAVE",

    /* TODO: Request to open a high-performance data stream */
    "CONNECT",

};

typedef struct socomm_header_t {
  /* Always initialize to "SOCOMM", or the message will be considered corrupt */
  uint8_t preamble[3];

  /* Always initialize to SOCOMM_HEADER_VERSION */
  uint8_t version;

  /* Set to your node's server port */
  uint16_t port;

  /* Unused bytes of padding */
  uint8_t reserved[2];

  /* Set to your node's uuid, which should be a uuid4 - see uuid4_gen */
  uuid4_t uuid;

} socomm_header;

static_assert(sizeof(socomm_header) == 24,
              "socomm_header must be a fixed size.");

socomm_header                   socomm_header_init(uint16_t port, uuid4_t uuid);

typedef struct socomm_message_t socomm_message;

/**
 * @brief Create a `socomm_message`.
 * @details May fail if `strnlen(message_type) >= 8`
 *
 * @param header
 * @param message_type One of the values in SOCOMM_MESSAGE_TYPES.
 * @param message_data Pointer to data.
 * @param message_data_size Size of the input data.
 * @return Pointer to the newly allocated `socomm_message`, or `NULL` if
 * creation failed.
 */
socomm_message      *socomm_message_create(socomm_header header,
                                           const char   *message_type,
                                           void         *message_data,
                                           size_t        message_data_size);

void                 socomm_message_destroy(socomm_message **message);

const socomm_header *socomm_message_header(socomm_message *message);
const void          *socomm_message_data(socomm_message *message);
const size_t         socomm_message_data_size(socomm_message *message);

#endif
