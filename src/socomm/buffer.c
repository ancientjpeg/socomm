/**
 * @file buffer.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#include "buffer.h"
#include <stdlib.h>
#include <string.h>

typedef struct socomm_buffer_t {
  void  *data;
  size_t size;
} socomm_buffer;

socomm_buffer *socomm_buffer_create()
{
  socomm_buffer *s = malloc(sizeof(socomm_buffer));
  s->data          = NULL;
  s->size          = 0;
  return s;
}

socomm_buffer *socomm_buffer_create_size(size_t size)
{
  socomm_buffer *s = socomm_buffer_create();

  if (size) {
    s->data = malloc(size + 1);
    memset(s->data, 0, size + 1);
    s->size = size;
  }

  return s;
}

socomm_buffer *socomm_buffer_create_data(const char *data, size_t size)
{
  socomm_buffer *s = socomm_buffer_create_size(size);

  if (size) {
    memcpy(s, data, size);
  }

  return s;
}

void socomm_buffer_destroy(socomm_buffer **s)
{

  if (*s == NULL) {
    return;
  }

  if ((*s)->data != NULL) {
    free((*s)->data);
  }

  free(*s);
  *s = NULL;
}

void socomm_buffer_ensure_size(socomm_buffer *s, size_t size)
{
  if (socomm_buffer_size(s) >= size) {
    return;
  }

  /** @todo only memset after memcpy range */
  char *data_tmp = (char *)malloc(size + 1);
  memset(data_tmp, 0, size + 1);

  if (socomm_buffer_data(s) != NULL) {
    memcpy(data_tmp, s->data, s->size);
    free(s->data);
  }

  s->data = data_tmp;
  s->size = size;
}

void socomm_buffer_copy(socomm_buffer *dst, const char *data, size_t size)
{
  socomm_buffer_ensure_size(dst, size);
  memcpy(dst->data, data, size);
}

const void *socomm_buffer_data(socomm_buffer *s)
{
  return s->data;
}

size_t socomm_buffer_size(socomm_buffer *s)
{
  return s->size;
}
