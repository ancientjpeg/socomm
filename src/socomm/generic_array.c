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

  const size_t mask = ((size_t)1) << (sizeof(size_t) * 8 - 1);
  /** @todo IMPORTANT handle these cases better */
  assert(!(mask & reserve));
  assert(!(mask & array->cap));

  /**
   * I know this is "faster" with __builtin_clz but the compiler is smart enough
   * to do that.
   */
  size_t reserve_tmp = 1;
  while (reserve_tmp < reserve) {
    reserve_tmp *= 2;
  }

  const size_t new_cap = array->cap == 0 ? reserve_tmp : array->cap * 2;

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
