/**
 * @file generic_array.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2025-02-01
 * Copyright (c) 2025. All rights reserved.
 */

#include "generic_array.h"
#include <assert.h>
#include <limits.h>
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
  assert(element_size != 0);

  socomm_array *array = malloc(sizeof(socomm_array));

  array->data         = NULL;
  array->element_size = element_size;
  array->len          = 0;
  array->cap          = 0;

  socomm_array_reserve(array, reserve);

  assert(array->cap >= reserve);

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

void socomm_array_reserve(socomm_array *array, size_t reserve)
{

  if (array->cap >= reserve) {
    return;
  }

  /* I think this is impossible but just in case I'm dumb */
  /** @todo: prove this with discrete math lol */
  assert(reserve != 0);

  /* let the compiler turn this into __builtin_clz */
  /** @todo find a smarter, more concise way to write this */
  size_t reserve_tmp;
  size_t mask = ((size_t)1) << (8 * sizeof(size_t) - 1);
  size_t lz   = 0;
  while (!(reserve_tmp & mask)) {
    reserve_tmp <<= 1;
    ++lz;
  }

  const size_t new_cap = array->cap == 0 ? 1 : array->cap * 2;

  array->data          = realloc(array->data, reserve * array->element_size);
  array->cap           = new_cap;

  /** @todo: gracefully handle out-of-memory situations ? */
  assert(array->data != NULL);
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
