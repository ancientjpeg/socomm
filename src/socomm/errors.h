/**
 * @file errors.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2025-02-01
 * Copyright (c) 2025. All rights reserved.
 */

#include <assert.h>

/**
 * @typedef socomm_error_t
 * @brief SOCOMM error codes. These values are not stable.
 * The one exception is that SOCOMM_SUCCESS will always be 0.
 */
typedef enum socomm_error_t {
  SOCOMM_SUCCESS        = 0,
  SOCOMM_ERROR          = -1,
  SOCOMM_ALREADY_EXISTS = -2,
  SOCOMM_DOES_NOT_EXIST = -3,
  SOCOMM_PORT_IN_USE    = -4,
} socomm_error_t;

static_assert(SOCOMM_SUCCESS == 0, "SOCOMM_SUCCESS is guaranteed to be 0.");
