/**
 * @file header.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#include "header.h"
#include "header_serde.h"
#include <stdlib.h>
#include <string.h>

#define SOCOMM_MESSAGE_TYPE_MAX_BYTES 8
#define SOCOMM_MAX_GROUP_NAME_BYTES (SOCOMM_MAX_GROUP_NAME_LENGTH + 1)

typedef struct socomm_message_t {
  /* All messages must contain a header. */
  socomm_header header;

  /* use message type to discern intent */
  char          message_type[SOCOMM_MESSAGE_TYPE_MAX_BYTES];

  uint64_t      data_size;
  uint64_t      data; /* to be cast to a void* */
} socomm_message;

static const int predicted_msg_size
    = (sizeof(socomm_header) + sizeof(void *) + sizeof(size_t)
       + SOCOMM_MESSAGE_TYPE_MAX_BYTES * sizeof(uint8_t));

static_assert(sizeof(socomm_message) == predicted_msg_size,
              "Unexpected socomm header sizing");

static_assert(sizeof(uintptr_t) <= sizeof(uint64_t),
              "Systems greater than 64 bit not supported.");

static_assert(offsetof(socomm_message, header) == 0,
              "Header should be first component of `socomm_message`.");

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

socomm_header
socomm_header_init(uint16_t port, uuid4_t node_uuid, const char *group_name)
{

  const int group_name_len = strlen(group_name);
  assert(group_name_len <= SOCOMM_MAX_GROUP_NAME_LENGTH);

  socomm_header header;
  header.preamble[0] = 'S';
  header.preamble[1] = 'C';
  header.preamble[2] = 'M';
  header.version     = SOCOMM_HEADER_VERSION;

  header.port        = port;
  header.uuid        = node_uuid;

  memset(header.group_name, '\0', SOCOMM_MAX_GROUP_NAME_BYTES);
  memcpy(header.group_name, group_name, group_name_len);

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
  memset(message->message_type, 0, SOCOMM_MESSAGE_TYPE_MAX_BYTES);
  memcpy(message->message_type, message_type, strlen(message_type));

  if (message_data == NULL || message_data_size == 0) {
    message->data = (uint64_t)NULL;
  }
  else {
    void *data = malloc(message_data_size);
    memcpy(data, message_data, message_data_size);
    message->data = (uint64_t)data;
  }

  message->data_size = message_data_size;

  return message;
}

void socomm_message_destroy(socomm_message **message)
{
  if (*message == NULL) {
    return;
  }

  const void *msg_data = socomm_message_data(*message);
  if (msg_data != NULL) {
    free((void *)msg_data);
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
  return (const void *)message->data;
}

const size_t socomm_message_data_size(socomm_message *message)
{
  return message->data_size;
}

/******************************************************************************/
/*******************    SERIALIZATION / DESERIALIZATION   *********************/
/******************************************************************************/
int socomm_serialize_message(socomm_message *msg, zmq_msg_t *out_msg)
{

  const int   header_size   = sizeof(socomm_header);

  const void *msg_data      = socomm_message_data(msg);
  uint64_t    msg_data_size = socomm_message_data_size(msg);

  zmq_msg_close(out_msg);

  if (msg_data == NULL) {
    assert(false);
    return -1;
  }

  const int message_type_offset = offsetof(socomm_message, message_type);
  const int data_size_offset    = offsetof(socomm_message, data_size);
  const int data_offset         = offsetof(socomm_message, data);
  int       zmq_msg_size        = data_offset + msg_data_size;

  zmq_msg_init_size(out_msg, zmq_msg_size);
  void    *zmq_msg_data_ptr = zmq_msg_data(out_msg);

  uint8_t *zp               = (uint8_t *)zmq_msg_data_ptr;
  uint8_t *mp               = (uint8_t *)msg;

  memcpy(zp, mp, header_size);
  memcpy(zp + message_type_offset,
         mp + message_type_offset,
         SOCOMM_MESSAGE_TYPE_MAX_BYTES);
  memcpy(zp + data_size_offset, mp + data_size_offset, sizeof(uint64_t));
  if (msg_data_size > 0) {
    memcpy(zp + data_offset, *(void **)(mp + data_offset), msg_data_size);
  }

  return 0;
}

socomm_message *socomm_deserialize_message(zmq_msg_t *msg)
{
  const void  *data = zmq_msg_data(msg);
  const size_t size = zmq_msg_size(msg);

  if (data == NULL || size == 0) {
    assert(false);
    return NULL;
  }

  socomm_header header = *(socomm_header *)data;
  char *message_type = (char *)(data + offsetof(socomm_message, message_type));
  uint64_t data_size
      = *(uint64_t *)(data + offsetof(socomm_message, data_size));
  const void *data_ptr = (void *)(data + offsetof(socomm_message, data));

  if (memcmp(header.preamble, "SCM", 3)
      || header.version != SOCOMM_HEADER_VERSION) {
    assert(false);
    return NULL;
  }

  return socomm_message_create(header, message_type, data_ptr, data_size);
}
