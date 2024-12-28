/**
 * @file test_helpers.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-29
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_TEST_HELPERS_H_
#define SOCOMM_TEST_HELPERS_H_

#include "socomm/header.h"

#ifndef SOCOMM_VERBOSE_TESTS
#define debug_printf(...)
#else
#define debug_printf printf
#endif

socomm_header get_test_header();

#endif
