/**
 * @file str.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#include "str.h"
#include <stdlib.h>
#include <string.h>

typedef struct socomm_string_t {
  void  *data;
  size_t size;
} socomm_string;

socomm_string *socomm_string_create()
{
  socomm_string *s = malloc(sizeof(socomm_string));
  s->data          = NULL;
  s->size          = 0;
  return s;
}

socomm_string *socomm_string_create_size(size_t size)
{
  socomm_string *s = socomm_string_create();

  if (size) {
    s->data = malloc(size + 1);
    memset(s->data, 0, size + 1);
    s->size = size;
  }

  return s;
}

socomm_string *socomm_string_create_data(const char *data, size_t size)
{
  socomm_string *s = socomm_string_create_size(size);

  if (size) {
    memcpy(s->data, data, size);
  }

  return s;
}

void socomm_string_destroy(socomm_string **s)
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

void socomm_string_ensure_size(socomm_string *s, size_t size)
{
  if (socomm_string_size(s) >= size) {
    return;
  }

  /** @todo only memset after memcpy range */
  char *data_tmp = (char *)malloc(size + 1);
  memset(data_tmp, 0, size + 1);

  if (socomm_string_data(s) != NULL) {
    memcpy(data_tmp, s->data, s->size);
    free(s->data);
  }

  s->data = data_tmp;
  s->size = size;
}

void socomm_string_copy(socomm_string *dst, const char *data, size_t size)
{
  socomm_string_ensure_size(dst, size);
  memcpy(dst->data, data, size);
}

const void *socomm_string_data(socomm_string *s)
{
  return s->data;
}

size_t socomm_string_size(socomm_string *s)
{
  return s->size;
}
