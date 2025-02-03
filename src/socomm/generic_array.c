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

#ifndef SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX
#define SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX 16
#endif

typedef struct socomm_array_t {
  void                             *data;
  size_t                            element_size;
  size_t                            len;
  size_t                            cap;
  socomm_array_element_destructor_t dtor;
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
  array->dtor         = NULL;

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

void socomm_array_set_element_destructor(socomm_array *array,
                                         socomm_array_element_destructor_t dtor)
{
  array->dtor = dtor;
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

void *socomm_array_push_back(socomm_array *array, void *element)
{
  return socomm_array_insert_at(array, element, array->len);
}

void *socomm_array_insert_at(socomm_array *array, void *element, size_t index)
{
  if (index > array->len) {
    return NULL;
  }

  socomm_array_reserve(array, array->len + 1);

  uint8_t *end              = socomm_array_at(array, array->len);

  uint8_t *element_location = socomm_array_at(array, index);
  uint8_t *move_dst         = element_location + array->element_size;
  size_t   move_size        = end - element_location;

  memmove(move_dst, element_location, move_size);
  memcpy(element_location, element, array->element_size);

  ++array->len;

  return element_location;
}

void *socomm_array_at(socomm_array *array, size_t index)
{
  assert(index <= array->len);
  return array->data + (index * array->element_size);
}

void *socomm_array_find(socomm_array *array, void *element)
{
  const bool static_element_size
      = array->element_size < SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX;

  for (size_t i = 0; i < array->len; ++i) {
    size_t data_offset = i * array->element_size;
    void  *cmp_data    = array->data + data_offset;
    if (memcmp(element, cmp_data, array->element_size) == 0) {
      return cmp_data;
    }
  }

  return NULL;
}

bool socomm_array_contains(socomm_array *array, void *element)
{
  return socomm_array_find(array, element) != NULL;
}

void socomm_array_pop_back(socomm_array *array)
{
  if (socomm_array_length(array) == 0) {
    return;
  }

  socomm_array_remove(array, array->len - 1);
}
void socomm_array_remove(socomm_array *array, size_t index)
{
  if (socomm_array_length(array) == 0) {
    return;
  }

  uint8_t *target_element = (uint8_t *)socomm_array_at(array, index);
  if (target_element == NULL) {
    return;
  }

  uint8_t *target_replacement = target_element + array->element_size;
  size_t   data_size          = array->len * array->element_size;
  uint8_t *end                = array->data + data_size;
  size_t   data_move_size     = end - target_replacement;

  if (array->dtor != NULL) {
    array->dtor(target_element, array->element_size);
  }

  memmove(target_element, target_replacement, data_move_size);

  --array->len;
}

void *socomm_array_element_at(socomm_array *array, size_t index)
{
  size_t data_offset = index * array->element_size;
  return array->data + data_offset;
}

void *socomm_array_element_at_checked(socomm_array *array, size_t index)
{
  if (index >= array->len) {
    return NULL;
  }

  return socomm_array_element_at(array, index);
}

size_t socomm_array_length(socomm_array *array)
{
  return array->len;
}

size_t socomm_array_capacity(socomm_array *array)
{
  return array->cap;
}
