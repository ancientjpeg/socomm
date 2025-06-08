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

static void socomm_verify_comparator(socomm_array_comp_t *comp)
{
  if (*comp == NULL) {
    *comp = memcmp;
  }
}

typedef struct socomm_array_t {
  void               *data;
  size_t              element_size;
  size_t              len;
  size_t              cap;
  socomm_array_dtor_t dtor;
} socomm_array;

socomm_array *socomm_array_create(size_t element_size)
{
  return socomm_array_create_reserve(element_size, 0);
}

socomm_array *socomm_array_create_reserve(size_t element_size, size_t reserve)
{

  if (element_size == 0) {
    return NULL;
  }

  socomm_array *array = malloc(sizeof(socomm_array));

  array->data         = NULL;
  array->element_size = element_size;
  array->len          = 0;
  array->cap          = 0;
  array->dtor         = NULL;

  socomm_array_reserve(array, reserve);

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

void socomm_array_set_dtor(socomm_array *array, socomm_array_dtor_t dtor)
{
  array->dtor = dtor;
}

void socomm_array_reserve(socomm_array *array, size_t reserve)
{

  if (array->cap >= reserve) {
    return;
  }

  const size_t reserve_bytes = reserve * array->element_size;
  const size_t z_bits        = sizeof(size_t) * 8;

  /* I think this is impossible but just in case I'm dumb */
  /** @todo: prove this with discrete math lol */
  assert(reserve != 0);

  /**
   * no point in manually using __builtin_clz when the compiler is always
   * smart enough to figure it out anyways.
   */
  size_t reserve_clz = 0;
  for (size_t clz_mask = (1UL << (z_bits - 1)); !(reserve_bytes & clz_mask);
       clz_mask >>= 1) {
    reserve_clz++;
  }

  size_t new_cap_bytes;
  if (reserve_clz == 0) {
    new_cap_bytes = reserve;
  }
  else {
    new_cap_bytes = 1 << (z_bits - reserve_clz);
  }

  array->data = realloc(array->data, new_cap_bytes);
  array->cap  = new_cap_bytes / array->element_size;

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

  /**
   * This does permit aliased insertion, i.e. `element` may be equal to
   * `element_location`.
   */
  memmove(move_dst, element_location, move_size);
  memcpy(element_location, element, array->element_size);

  ++array->len;

  return element_location;
}

bool socomm_array_pop_back(socomm_array *array)
{
  if (socomm_array_length(array) == 0) {
    return false;
  }

  return socomm_array_remove(array, array->len - 1);
}

void socomm_array_clear(socomm_array *array)
{
  if (array->dtor != NULL) {
    for (size_t i = 0; i < array->len; ++i) {
      void *data = socomm_array_at(array, i);
      array->dtor(data, array->element_size);
    }
  }
  array->len = 0;
}

bool socomm_array_remove(socomm_array *array, size_t index)
{
  if (index >= array->len) {
    return false;
  }

  uint8_t *target_element     = (uint8_t *)socomm_array_at(array, index);
  uint8_t *target_replacement = target_element + array->element_size;
  uint8_t *end            = socomm_array_at(array, socomm_array_length(array));
  size_t   data_move_size = end - target_replacement;

  if (array->dtor != NULL) {
    array->dtor(target_element, array->element_size);
  }

  if (target_element != end) {
    memmove(target_element, target_replacement, data_move_size);
  }

  --array->len;

  return true;
}

void socomm_array_element_swap(void *a, void *b, size_t size)
{

  if (size <= SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX) {
    uint8_t temp[SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
  }
  else {
    /* @todo Profile this - performance probably sucks but its an edge case */
    while (size--) {
      uint8_t *a_byte = (uint8_t *)a++;
      uint8_t *b_byte = (uint8_t *)b++;
      uint8_t  tmp;

      tmp     = *a_byte;
      *a_byte = *b_byte;
      *b_byte = tmp;
    }
  }
}

size_t
socomm_array_purge(socomm_array *array, void *element, socomm_array_comp_t comp)
{

  uint8_t    element_buff_static[SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX];
  uint8_t   *element_buff = element_buff_static;

  const bool use_dynamic_element_buff
      = array->element_size > SOCOMM_ARRAY_ELEMENT_STATIC_SIZE_MAX;

  /** @todo maybe refactor this into a check */
  if (use_dynamic_element_buff) {
    element_buff = malloc(array->element_size);
  }

  memcpy(element_buff, element, array->element_size);

  socomm_verify_comparator(&comp);

  /**
   * Yes, Jackson, I know this can underflow, but if it does the
   * loop won't run. Calm down.
   * - Jackson
   */
  uint8_t *swap_target
      = (uint8_t *)socomm_array_at(array, socomm_array_length(array) - 1);
  size_t removed = 0;

  for (size_t i = 0; i < socomm_array_length(array);) {

    void *current_element = socomm_array_at(array, i);

    if (comp(current_element, element_buff, array->element_size) == 0) {
      socomm_array_element_swap(current_element,
                                swap_target,
                                array->element_size);
      socomm_array_pop_back(array);
      ++removed;

      /**
       * @todo I'm not 100% confident in this. My gut says it won't, but
       * write out a proof to be sure.
       */
      swap_target -= array->element_size;
      continue;
    }

    ++i;
  }

  if (use_dynamic_element_buff) {
    free(element_buff);
  }

  return removed;
}

void *socomm_array_at(socomm_array *array, size_t index)
{
  return array->data + (index * array->element_size);
}

const void *socomm_array_at_const(socomm_array *array, size_t index)
{
  return socomm_array_at(array, index);
}

size_t socomm_array_index_of(const socomm_array *array,
                             void               *element,
                             socomm_array_comp_t comp)
{
  void *found = socomm_array_find(array, element, comp);
  return found == NULL ? socomm_array_length(array)
                       : (found - array->data) / array->element_size;
}

void *socomm_array_find(const socomm_array *array,
                        void               *element,
                        socomm_array_comp_t comp)
{

  socomm_verify_comparator(&comp);

  for (size_t i = 0; i < array->len; ++i) {
    size_t data_offset = i * array->element_size;
    void  *cmp_data    = array->data + data_offset;
    if (comp(element, cmp_data, array->element_size) == 0) {
      return cmp_data;
    }
  }

  return NULL;
}

bool socomm_array_contains(socomm_array       *array,
                           void               *element,
                           socomm_array_comp_t comp)
{
  return socomm_array_find(array, element, comp) != NULL;
}

size_t socomm_array_length(const socomm_array *array)
{
  return array->len;
}

size_t socomm_array_capacity(const socomm_array *array)
{
  return array->cap;
}
