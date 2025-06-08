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

/**
 * @brief `socomm_array` is a general-purpose linear array, similar to
 * C++'s std::vector. It handles basic buffer allocation as well as
 * provides functionality for search, insertion, and removal. None of these
 * methods are guaranteed thread-safe; all method calls must be protected
 * by synchronization mechanisms in multi-threaded contexts.
 *
 */
typedef struct socomm_array_t socomm_array;
typedef void (*socomm_array_dtor_t)(void *element, size_t element_size);

typedef int (*socomm_array_comp_t)(const void *a,
                                   const void *b,
                                   size_t      element_size);

/**
 * @brief Creates an array to contain elements of `element_size`.
 * @details Returns NULL iff element_size == 0, or malloc fails. Note that
 * `element_size` does not just control the initial allocation, but also
 * dictates the internal functioning of the array.
 */
socomm_array *socomm_array_create(size_t element_size);
socomm_array *socomm_array_create_reserve(size_t element_size, size_t reserve);

void          socomm_array_destroy(socomm_array **array);

/**
 * @brief Set a destructor for elements of this array. Useful when holding an
 * array of allocated objects.
 */
void   socomm_array_set_dtor(socomm_array *array, socomm_array_dtor_t dtor);

/**
 * @brief Reserve memory for at least `reserve` elements.
 */
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
 * @brief Quickly clears the array and sets size to 0. Will not perform
 * reallocation or clear the data buffer.
 *
 * If you'd like to deallocate memory but keep the array alive,
 * use `socomm_array_destroy` immediately followed by
 * `socomm_array_create`.
 */
void   socomm_array_clear(socomm_array *array);

/**
 * @brief Removes the element at `index`. Maintains stable ordering of
 * other array elements. Returns `false` if `index` is out-of-bounds.
 */
bool   socomm_array_remove(socomm_array *array, size_t index);

/**
 * @brief Removes the last element of `array`. Returns `false` if array
 * was already empty or the removal fails for some other reason.
 */
bool   socomm_array_pop_back(socomm_array *array);

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

/**
 * @brief Unchecked method to retrieve a pointer to the data stored at `index`.
 * @note Valid to use this method to retrieve a past-the-end iteration pointer
 * by calling `socomm_array_at(array, socomm_array_length(array))`
 *
 * @param array
 * @param index
 * @return The pointer to element `index` of `array`. This pointer should
 * be treated as an iterator, and may be invalidated by any non-const
 * array methods, i.e. those that modify size/capacity/ordering.
 */
void  *socomm_array_at(socomm_array *array, size_t index);
const void *socomm_array_at_const(socomm_array *array, size_t index);

/**
 * @brief Returns the index of `element` in `array` if it is found using
 * `comp` as a comparator. If the element is not found, returns
 * `socomm_array_length(array)`.
 */
size_t      socomm_array_index_of(const socomm_array *array,
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
void       *socomm_array_find(const socomm_array *array,
                              void               *element,
                              socomm_array_comp_t comp);

/**
 * @brief `true` if `array` contains `element` when compared using `comp`.
 */
bool        socomm_array_contains(socomm_array       *array,
                                  void               *element,
                                  socomm_array_comp_t comp);

/**
 * @return Number of elements currently stored in `array`.
 */
size_t      socomm_array_length(const socomm_array *array);

/**
 * @return Number of elements that `array` currently has allocated space for.
 */
size_t      socomm_array_capacity(const socomm_array *array);

#endif
