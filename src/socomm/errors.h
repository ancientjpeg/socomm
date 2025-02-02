/**
 * @file errors.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2025-02-01
 * Copyright (c) 2025. All rights reserved.
 */

/**
 * @typedef socomm_error_t
 * @brief SOCOMM error codes. These values are not stable.
 *
 */
typedef enum socomm_error_t {
  SOCOMM_SUCCESS        = 0,
  SOCOMM_ALREADY_EXISTS = -1,
  SOCOMM_DOES_NOT_EXIST = -2,
  SOCOMM_PORT_IN_USE    = -3,
} socomm_error_t;
