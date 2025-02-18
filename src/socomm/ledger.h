/**
 * @file ledger.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-27
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_LEDGER_H_
#define SOCOMM_LEDGER_H_

#include "uuid/uuid.h"
#include <stddef.h>

typedef struct socomm_ledger_t socomm_ledger;

socomm_ledger                 *socomm_ledger_create();
socomm_ledger                 *socomm_ledger_create_reserve(size_t reserve);

void                           socomm_ledger_destroy(socomm_ledger **ledger);

/**
 * @brief Adds a uuid/port combination to the ledger. Will fail if `uuid` exists
 * in the ledger, *even if the port is different*. To add a uuid with a
 * different port number, please manually remove it first.
 *
 * @param uuid
 * @param port
 * @return 0 on success. May return the following standard errnos:
 * - SOCOMM_ALREADY_EXISTS: Entry already exists.
 * - SOCOMM_PORT_IN_USE: An entry is already using this port. May indicate
 * something has gone wrong with node management.
 */
int socomm_ledger_add_entry(socomm_ledger *ledger, uuid4_t uuid, uint16_t port);

bool socomm_ledger_entry_exists(socomm_ledger *ledger, uuid4_t uuid);

/**
 * @brief Removes the entry corr
 *
 * @param uuid
 * @param port
 * @return 0 on successful removal, or if `uuid` does not exist in ledger.
 */
int  socomm_ledger_remove_entry(socomm_ledger *ledger, uuid4_t uuid);

#endif
