/**
 * @file buffer.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-23
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_BUFFER_H_
#define SOCOMM_BUFFER_H_

#include <stddef.h>

typedef struct socomm_buffer_t socomm_buffer;

socomm_buffer                 *socomm_buffer_create();
socomm_buffer                 *socomm_buffer_create_size(size_t size);

/**
 * @brief Creates string of size `size` and copy `data` into it.
 */
socomm_buffer *socomm_buffer_create_data(const char *data, size_t size);
void           socomm_buffer_destroy(socomm_buffer **s);

void           socomm_buffer_ensure_size(socomm_buffer *s, size_t size);
void socomm_buffer_copy(socomm_buffer *dst, const char *data, size_t size);

const void *socomm_buffer_data(socomm_buffer *s);
size_t      socomm_buffer_size(socomm_buffer *s);

#endif
