/**
 * @file ledger.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-27
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/ledger.h"
#include "uuid/uuid.h"
#include <stdlib.h>

typedef struct socomm_ledger_t {
  uuid4_t *uuids;
  size_t   len;
  size_t   cap;
} socomm_ledger_t;

socomm_ledger_t *socomm_ledger_create()
{
  socomm_ledger_t *ledger             = malloc(sizeof(socomm_ledger_t));
  const int        initial_ledger_cap = 10;

  ledger->uuids = malloc(initial_ledger_cap * sizeof(uuid4_t));
  ledger->len   = 0;
  ledger->cap   = 10;

  return ledger;
}

void socomm_ledger_destroy(socomm_ledger_t **ledger)
{
  free((*ledger)->uuids);
  free(*ledger);
  *ledger = NULL;
}
