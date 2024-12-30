/**
 * @file ledger.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-27
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_LEDGER_H_
#define SOCOMM_LEDGER_H_

typedef struct socomm_ledger_t socomm_ledger_t;

socomm_ledger_t               *socomm_ledger_create();

void                           socomm_ledger_destroy(socomm_ledger_t **ledger);

#endif
