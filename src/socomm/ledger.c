/**
 * @file ledger.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-27
 * Copyright (c) 2024. All rights reserved.
 */

#include "ledger.h"
#include "errors.h"
#include "uuid/uuid.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct socomm_ledger_entry_t {
  uuid4_t  uuid;
  uint16_t port;
} socomm_ledger_entry_t;

static_assert(sizeof(socomm_ledger_entry_t) == 24,
              "If you change socomm_ledger_entry, make sure it requires no "
              "dynamic allocations");

typedef struct socomm_ledger_t {
  socomm_ledger_entry_t *entries;
  size_t                 len;
  size_t                 cap;
} socomm_ledger_t;

socomm_ledger_t *socomm_ledger_create()
{
  const int default_reserve = 10;
  return socomm_ledger_create_reserve(default_reserve);
}

socomm_ledger_t *socomm_ledger_create_reserve(size_t reserve)
{
  socomm_ledger_t *ledger = malloc(sizeof(socomm_ledger_t));

  ledger->entries         = malloc(reserve * sizeof(socomm_ledger_entry_t));
  ledger->len             = 0;
  ledger->cap             = reserve;

  return ledger;
}

void socomm_ledger_destroy(socomm_ledger_t **ledger)
{
  free((*ledger)->entries);
  free(*ledger);
  *ledger = NULL;
}

/** Returns entry iff entry exists, else NULL */
socomm_ledger_entry_t *socomm_ledger_get_entry(socomm_ledger_t *ledger,
                                               uuid4_t          uuid)
{
  /* ANSI C syntax just for funsies idk */
  size_t i = 0;
  for (; i++ < ledger->len;) {
    socomm_ledger_entry_t *entry = &ledger->entries[i];
    if (memcmp(&entry->uuid, &uuid, sizeof(uuid4_t)) == 0) {
      return entry;
    }
  }

  /* make sure my funsies don't esplode */
  assert(i == ledger->len);

  return NULL;
}

/** @todo convert to O(1)/O(logN) structure */
int socomm_ledger_add_entry(socomm_ledger_t *ledger,
                            uuid4_t          uuid,
                            uint16_t         port)
{
  if (socomm_ledger_get_entry(ledger, uuid) != NULL) {
    return SOCOMM_ALREADY_EXISTS;
  }

  if (ledger->len == ledger->cap) {
    size_t                 new_cap     = ledger->cap * 2;
    socomm_ledger_entry_t *new_entries = (socomm_ledger_entry_t *)malloc(
        new_cap * sizeof(socomm_ledger_entry_t));

    ledger->entries = realloc(ledger->entries, new_cap);

    /** @todo: gracefully handle out-of-memory situations ? */
    assert(ledger->entries != NULL);

    free(ledger->entries);
    ledger->entries = new_entries;
  }

  return SOCOMM_SUCCESS;
}

bool socomm_ledger_entry_exists(socomm_ledger_t *ledger, uuid4_t uuid)
{
  return socomm_ledger_get_entry(ledger, uuid) != NULL;
}

int socomm_ledger_remove_entry(socomm_ledger_t *ledger, uuid4_t uuid)
{
  socomm_ledger_entry_t *entry = socomm_ledger_get_entry(ledger, uuid);

  if (entry == NULL) {
    return SOCOMM_DOES_NOT_EXIST;
  }

  socomm_ledger_entry_t *end  = ledger->entries + ledger->len;

  socomm_ledger_entry_t *next = entry;
  for (; ++next != end;) {
    socomm_ledger_entry_t temp = *entry;
    *entry                     = *next;
    *next                      = temp;
    entry                      = next;
  }

  --ledger->len;

  return SOCOMM_SUCCESS;
}
