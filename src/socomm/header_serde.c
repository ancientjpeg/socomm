/**
 * @file header_serde.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-24
 * Copyright (c) 2024. All rights reserved.
 */
#include "header_serde.h"

zmq_msg_t serialize_message(socomm_message *msg)
{
  const int header_size = sizeof(socomm_header);
}

socomm_message *deserialize_message(zmq_msg_t *msg)
{
  const void  *data = zmq_msg_data(msg);
  const size_t size = zmq_msg_size(msg);
  if (data == NULL || size == 0) {
  }
}
