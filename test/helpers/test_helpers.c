/**
 * @file test_helpers.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-29
 * Copyright (c) 2024. All rights reserved.
 */

#include "test_helpers.h"

socomm_header get_test_header()
{
  uuid4_state_t uuid_state;
  uuid4_t       uuid;
  uuid4_seed(&uuid_state);
  uuid4_gen(&uuid_state, &uuid);

  return socomm_header_init(9350, uuid);
}
