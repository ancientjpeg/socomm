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

socomm_array                 *socomm_array_create(size_t element_size);
socomm_array *socomm_array_create_reserve(size_t element_size, size_t reserve);
void          socomm_array_destroy(socomm_array **array);

void          socomm_array_insert(socomm_array *array, void *element);
bool          socomm_array_contains(socomm_array *array, void *element);
void          socomm_array_remove(socomm_array *array, void *element);

#endif
