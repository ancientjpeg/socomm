/**
 * @file ledger.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-27
 * Copyright (c) 2024. All rights reserved.
 */

#include "ledger.h"
#include "errors.h"
#include "generic_array.h"
#include "uuid/uuid.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct socomm_ledger_entry_t {
  uuid4_t  uuid;
  uint16_t port;
} socomm_ledger_entry;

int socomm_ledger_comp(const void *a, const void *b, size_t size)
{
  socomm_ledger_entry *entry_a = (socomm_ledger_entry *)a;
  socomm_ledger_entry *entry_b = (socomm_ledger_entry *)b;

  return memcmp(&entry_a->uuid, &entry_b->uuid, sizeof(uuid4_t));
}

int socomm_ledger_port_comp(const void *a, const void *b, size_t size)
{
  socomm_ledger_entry *entry_a = (socomm_ledger_entry *)a;
  socomm_ledger_entry *entry_b = (socomm_ledger_entry *)b;

  return entry_a->port == entry_b->port;
}

static_assert(sizeof(socomm_ledger_entry) == 24,
              "If you change socomm_ledger_entry, make sure it requires no "
              "dynamic allocations");

typedef struct socomm_ledger_t {
  socomm_array *entries;
} socomm_ledger;

socomm_ledger *socomm_ledger_create()
{
  const int default_reserve = 10;
  return socomm_ledger_create_reserve(default_reserve);
}

socomm_ledger *socomm_ledger_create_reserve(size_t reserve)
{
  socomm_ledger *ledger = malloc(sizeof(socomm_ledger));

  ledger->entries
      = socomm_array_create_reserve(sizeof(socomm_ledger_entry), reserve);
  socomm_array_set_comp(ledger->entries, socomm_ledger_comp);

  return ledger;
}

void socomm_ledger_destroy(socomm_ledger **ledger)
{
  free((*ledger)->entries);
  free(*ledger);
  *ledger = NULL;
}

/** Returns entry iff entry exists, else NULL */
socomm_ledger_entry *socomm_ledger_get_entry(socomm_ledger *ledger,
                                             uuid4_t        uuid)
{
  socomm_ledger_entry dummy_entry;
  dummy_entry.uuid = uuid;
  return socomm_array_find(ledger->entries, &dummy_entry);
}

/** @todo convert to O(1)/O(logN) structure */
int socomm_ledger_add_entry(socomm_ledger *ledger, uuid4_t uuid, uint16_t port)
{
  if (socomm_ledger_get_entry(ledger, uuid) != NULL) {
    return SOCOMM_ALREADY_EXISTS;
  }

  /** @todo make this more efficient */
  for (size_t i = 0; i < socomm_array_length(ledger->entries); ++i) {
    socomm_ledger_entry *entry = socomm_array_at(ledger->entries, i);
    if (memcmp(&entry->uuid, &uuid, sizeof(uuid4_t))) {
      return SOCOMM_ALREADY_EXISTS;
    }
    if (entry->port == port) {
      return SOCOMM_PORT_IN_USE;
    }
  }

  socomm_ledger_entry new_entry = {uuid, port};
  return socomm_array_push_back(ledger->entries, &new_entry) != NULL
             ? SOCOMM_SUCCESS
             : SOCOMM_ERROR;
}

bool socomm_ledger_entry_exists(socomm_ledger *ledger, uuid4_t uuid)
{
  return socomm_ledger_get_entry(ledger, uuid) != NULL;
}

int socomm_ledger_remove_entry(socomm_ledger *ledger, uuid4_t uuid)
{
  socomm_ledger_entry *entry = socomm_ledger_get_entry(ledger, uuid);

  // socomm_array_remove_if

  //     if (entry == NULL)
  // {
  //   return SOCOMM_DOES_NOT_EXIST;
  // }

  // socomm_ledger_entry *end  = ledger->entries + ledger->len;

  // socomm_ledger_entry *next = entry;
  // for (; ++next != end;) {
  //   socomm_ledger_entry temp = *entry;
  //   *entry                   = *next;
  //   *next                    = temp;
  //   entry                    = next;
  // }

  // --ledger->len;

  return SOCOMM_SUCCESS;
}
