/**
 * @file generic_array.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2025-02-01
 * Copyright (c) 2025. All rights reserved.
 */

#include "generic_array.h"
#include <stdlib.h>
#include <string.h>

typedef struct socomm_array_t {
  void  *data;
  size_t element_size;
  size_t len;
  size_t cap;
} socomm_array;

socomm_array *socomm_array_create(size_t element_size)
{
  return socomm_array_create_reserve(element_size, 0);
}

socomm_array *socomm_array_create_reserve(size_t element_size, size_t reserve)
{
  socomm_array *array = malloc(sizeof(socomm_array));

  if (reserve == 0) {
    array->data = NULL;
  }
  else {
    array->data = malloc(element_size * reserve);
  }

  array->element_size = element_size;
  array->len          = 0;
  array->cap          = reserve;

  return array;
}

void socomm_array_destroy(socomm_array **array)
{
  socomm_array *a = *array;

  if (a == NULL) {
    return;
  }

  if (a->data) {
    free(a->data);
  }

  free(a);

  *array = NULL;
}

void socomm_array_insert(socomm_array *array, void *element)
{
}

bool socomm_array_contains(socomm_array *array, void *element)
{
}

void socomm_array_remove(socomm_array *array, void *element)
{
}
