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

#define SOCOMM_HEADER_VERSION 0
static_assert(SOCOMM_HEADER_VERSION <= UINT8_MAX,
              "If SOCOMM_HEADER_VERSION has exceeded the range of a uint8_t, "
              "please panic.");

typedef enum socomm_message_t : uint8_t {
  /* Signifies a UDP heartbeat. */
  PULSE,

  /* Request to enter a group. */
  JOIN,

  /* Request to leave a group. */
  LEAVE,

  /* TODO: Request to open a high-performance data stream */
  CONNECT,

} socomm_message_t;

static_assert(sizeof(socomm_message_t) == 1,
              "socomm_command_t must never exceed 1 byte");

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

typedef struct socomm_body_t {
  /* use message_t to discern intent */
  uint8_t message_type[8];
} socomm_body;

#endif
