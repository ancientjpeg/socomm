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
  char  *data;
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
  s->data          = malloc(size + 1);
  memset(s->data, 0, size + 1);
  s->size = size;
  return s;
}

socomm_string *socomm_string_create_data(const char *data, size_t size)
{
  socomm_string *s = socomm_string_create_size(size);
  memcpy(s, data, size);
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

const char *socomm_string_data(socomm_string *s)
{
  return s->data;
}

size_t socomm_string_size(socomm_string *s)
{
  return s->size;
}
