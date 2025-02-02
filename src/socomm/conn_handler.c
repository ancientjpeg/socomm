/**
 * @file conn_handler.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-30
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/conn_handler.h"
#include "socomm/ledger.h"
#include <stdlib.h>

typedef struct socomm_conn_handler_t {
  socomm_ledger *ledger;
} socomm_conn_handler_t;

socomm_conn_handler_t *socomm_conn_handler_create()
{
  socomm_conn_handler_t *handler
      = (socomm_conn_handler_t *)malloc(sizeof(socomm_conn_handler_t));

  handler->ledger = socomm_ledger_create();

  return handler;
}

void socomm_conn_handler_destroy(socomm_conn_handler_t **handler)
{
  socomm_ledger_destroy(&(*handler)->ledger);
  free(*handler);
  *handler = NULL;
}
