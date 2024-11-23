/**
 * @file helpers.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#include "helpers.h"
#include <stdatomic.h>
#include <stdbool.h>

static atomic_uintptr_t ctx;

void                    socomm_free_global_ctx()
{
  uintptr_t exchange = (uintptr_t)NULL;

  uintptr_t prev     = atomic_exchange(&ctx, exchange);
  if (prev != exchange) {
    zmq_ctx_destroy((void *)prev);
  }
}

void *socomm_get_global_ctx()
{

  if (atomic_load(&ctx) == (uintptr_t)NULL) {

    void     *ctx_tmp = zmq_ctx_new();
    uintptr_t comp    = (uintptr_t)NULL;
    bool      exchanged
        = atomic_compare_exchange_strong(&ctx, &comp, (uintptr_t)ctx_tmp);

    if (exchanged) {
      atexit(socomm_free_global_ctx);
    }
    else {
      zmq_ctx_destroy(ctx_tmp);
    }
  }

  /**
   * TODO optimize this to a single atomic load; currently written this way for
   * clarity.
   */
  return (void *)atomic_load(&ctx);
}
