/**
 * @file helpers.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_HELPERS_H_
#define SOCOMM_HELPERS_H_

#include <cstdlib>
#include <cstring>
#include <zmq.h>

namespace socomm {

namespace helpers {

inline void *get_global_zmq_ctx()
{

  static struct holder {
    holder() : ctx(zmq_ctx_new())
    {
    }

    ~holder()
    {
      zmq_ctx_destroy(ctx);
    }

    void *const ctx;

  } global_ctx_holder;

  return global_ctx_holder.ctx;
}

inline void handle_status(int status)
{
  if (status == 0) {
    return;
  }

  printf("ERROR: %s\n", zmq_strerror(status));
  exit(status);
}

inline void handle_errno(int status)
{
  if (status != -1) {
    return;
  }

  printf("ERROR: %s\n", strerror(errno));
  exit(errno);
}

} // namespace helpers

} // namespace socomm

#endif
