/**
 * @file header.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#include "header.h"
#include <stdlib.h>
#include <string.h>

#define SOCOMM_MESSAGE_TYPE_MAX_BYTES 8
typedef struct socomm_message_t {
  /* All messages must contain a header. */
  socomm_header header;

  /* use message_t to discern intent */
  uint8_t message_type[SOCOMM_MESSAGE_TYPE_MAX_BYTES];

  size_t  data_size;
  void   *data;
} socomm_message;

static_assert(sizeof(socomm_message)
                  == (sizeof(socomm_header) + sizeof(void *) + sizeof(size_t)
                      + 8 * sizeof(uint8_t)),
              "Unexpected socomm header sizing");

static inline bool socomm_valid_command(const char *message_type)
{

  const void *nullterm_ptr
      = memchr((void *)message_type, (int)'\0', SOCOMM_MESSAGE_TYPE_MAX_BYTES);

  if (nullterm_ptr == NULL) {
    return false;
  }

  int num_types
      = sizeof(SOCOMM_MESSAGE_TYPES) / sizeof(SOCOMM_MESSAGE_TYPES[0]);

  /** @todo make this a hash table if we exceed 10 or so command types */
  for (int i = 0; i < num_types; ++i) {
    const char *type = SOCOMM_MESSAGE_TYPES[i];
    if (strcmp(type, message_type) == 0) {
      return true;
    }
  }

  return false;
}

socomm_header socomm_header_init(uint16_t port, uuid4_t uuid)
{
  socomm_header header;
  header.preamble[0] = 'S';
  header.preamble[1] = 'C';
  header.preamble[2] = 'M';
  header.version     = SOCOMM_HEADER_VERSION;

  header.port        = port;
  header.uuid        = uuid;

  return header;
}

socomm_message *socomm_message_create(socomm_header header,
                                      const char   *message_type,
                                      const void   *message_data,
                                      size_t        message_data_size)
{

  if (!socomm_valid_command(message_type)) {
    return NULL;
  }

  socomm_message *message = malloc(sizeof(socomm_message));
  message->header         = header;
  memcpy(message->message_type, message_type, SOCOMM_MESSAGE_TYPE_MAX_BYTES);

  if (message_data == NULL || message_data_size == 0) {
    message->data = NULL;
  }
  else {
    message->data = malloc(message_data_size);
    memcpy(message->data, message_data, message_data_size);
  }

  message->data_size = message_data_size;

  return message;
}

void socomm_message_destroy(socomm_message **message)
{
  if (*message == NULL) {
    return;
  }

  if ((*message)->data != NULL) {
    free((*message)->data);
  }

  free(*message);
  *message = NULL;
}

const socomm_header *socomm_message_header(socomm_message *message)
{
  return &message->header;
}

const void *socomm_message_data(socomm_message *message)
{
  return message->data;
}

const size_t socomm_message_data_size(socomm_message *message)
{
  return message->data_size;
}
