/**
 * @file generic_array.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2025-02-01
 * Copyright (c) 2025. All rights reserved.
 */

#ifndef SOCOMM_GENERIC_ARRAY_H_
#define SOCOMM_GENERIC_ARRAY_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct socomm_array_t socomm_array;
typedef void (*socomm_array_dtor_t)(void *element, size_t element_size);

typedef int (*socomm_array_comp_t)(const void *a,
                                   const void *b,
                                   size_t      element_size);

/** Returns NULL iff element_size == 0, or malloc fails. */
socomm_array *socomm_array_create(size_t element_size);
socomm_array *socomm_array_create_reserve(size_t element_size, size_t reserve);

void          socomm_array_destroy(socomm_array **array);

void   socomm_array_set_dtor(socomm_array *array, socomm_array_dtor_t dtor);
void   socomm_array_set_comp(socomm_array *array, socomm_array_comp_t comp);

void   socomm_array_reserve(socomm_array *array, size_t reserve);

/**
 * @brief Push an element to the back of the array. Data will be allocated as
 * necessary.
 *
 * @param element Pointer to the data to copy. *MAY NOT ALIAS WITH DATA IN
 * `array`*
 * @param element
 * @return Pointer to the data stored in `array`, or `NULL` in the event of a
 * failure. **At the time of writing**, there are no events in which
 * this function will fail.
 */
void  *socomm_array_push_back(socomm_array *array, void *element);

/**
 * @brief Insert an element such that the data for `element` will be copied to
 * `index`.
 *
 * @param array
 * @param element Pointer to the data to copy. *MAY NOT ALIAS WITH DATA IN
 * `array`*
 * @param index The target index.
 * @return Pointer to the data stored in `array`, or `NULL` in the event of a
 * failure. This method will return NULL if
 * `index > socomm_array_length(array)`.
 * `socomm_array_insert_at(array, socomm_array_length(array))` is a special case
 * with identical behavior to `socomm_array_push_back`.
 */
void  *socomm_array_insert_at(socomm_array *array, void *element, size_t index);

/**
 * @brief Unchecked method to retrieve a pointer to the data stored at `index`.
 * @note Valid to use this method to retrieve a past-the-end iteration pointer
 * by calling `socomm_array_at(array, socomm_array_length(array))`
 *
 * @param array
 * @param index
 */
void  *socomm_array_at(socomm_array *array, size_t index);

void  *socomm_array_find(socomm_array *array, void *element);
bool   socomm_array_contains(socomm_array *array, void *element);

void   socomm_array_pop_back(socomm_array *array);
void   socomm_array_remove(socomm_array *array, size_t index);

void  *socomm_array_element_at(socomm_array *array, size_t index);
void  *socomm_array_element_at_checked(socomm_array *array, size_t index);

/**
 * @return Number of elements currently stored in `array`.
 */
size_t socomm_array_length(socomm_array *array);

/**
 * @return Number of elements that `array` currently has allocated space for.
 */
size_t socomm_array_capacity(socomm_array *array);

#endif
