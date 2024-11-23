/**
 * @file helpers.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_HELPERS_H_
#define SOCOMM_HELPERS_H_

#include <stdlib.h>
#include <string.h>
#include <zmq.h>

void       *socomm_get_global_ctx();
void        socomm_free_global_ctx();

inline void socomm_handle_status(int status)
{
  if (status == 0) {
    return;
  }

  printf("ERROR: %s\n", zmq_strerror(status));
  exit(status);
}

inline void socomm_handle_errno(int status)
{
  if (status != -1) {
    return;
  }

  printf("ERROR: %s\n", strerror(errno));
  exit(errno);
}

/* use to create simply malloc'd zmsg structures */
inline void socomm_compatible_free(void *data, void *hint)
{
  free(data);
}

#endif
