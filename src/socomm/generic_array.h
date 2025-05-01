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
 * @return The pointer to element `index` of `array`.
 */
void  *socomm_array_at(socomm_array *array, size_t index);

/**
 * @brief Returns the index of `element` in `array` if it is found using
 * `comp` as a comparator. If the element is not found, returns
 * `socomm_array_length(array)`.
 */
size_t socomm_array_index_of(socomm_array       *array,
                             void               *element,
                             socomm_array_comp_t comp);

/**
 * @brief Finds the first element in `array` that matches `element` and returns
 * it.
 *
 * @param array
 * @param element The element to find.
 * @param comp The comparator function. If `NULL`, `memcmp` is used.
 * @return The found element, or NULL if no matching element was found.
 */
void *
socomm_array_find(socomm_array *array, void *element, socomm_array_comp_t comp);

/**
 * @brief `true` if `array` contains `element` when compared using `comp`.
 */
bool   socomm_array_contains(socomm_array       *array,
                             void               *element,
                             socomm_array_comp_t comp);

/**
 * @brief Removes the last element of `array`. Returns `false` if array
 * was already empty or the removal fails for some other reason.
 */
bool   socomm_array_pop_back(socomm_array *array);

/**
 * @brief Removes the element at `index`. Maintains stable ordering of
 * other array elements. Returns `false` if `index` is out-of-bounds.
 */
bool   socomm_array_remove(socomm_array *array, size_t index);

/**
 * @brief Remove all elements equal to `element`. Is not stable; not
 * guaranteed to maintain the ordering of `array`, but is guaranteed
 * to perform at most O(n) swaps.
 *
 * @param array
 * @param element The element to delete.
 * @param comp The comparator function. If `NULL`, `memcmp` is used.
 * @return The number of elements removed.
 */
size_t socomm_array_purge(socomm_array       *array,
                          void               *element,
                          socomm_array_comp_t comp);

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
