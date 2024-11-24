/**
 * @file str.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_STRING_H_
#define SOCOMM_STRING_H_

#include <stddef.h>

typedef struct socomm_string_t socomm_string;

socomm_string                 *socomm_string_init();
socomm_string                 *socomm_string_init_size(size_t size);

/**
 * @brief Creates string of size `size` and copy `data` into it.
 */
socomm_string *socomm_string_create_data(const char *data, size_t size);
void           socomm_string_destroy(socomm_string **s);

void           socomm_string_ensure_size(socomm_string *s, size_t size);
void socomm_string_copy(socomm_string *dst, const char *data, size_t size);

const char *socomm_string_data(socomm_string *s);
size_t      socomm_string_size(socomm_string *s);

#endif
