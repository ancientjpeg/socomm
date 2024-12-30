/**
 * @file conn_handler.c
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-30
 * Copyright (c) 2024. All rights reserved.
 */

#include "socomm/conn_handler.h"
#include <stdlib.h>

typedef struct socomm_conn_handler_t {
} socomm_conn_handler_t;

socomm_conn_handler_t *socomm_conn_handler_create()
{
  socomm_conn_handler_t *handler
      = (socomm_conn_handler_t *)malloc(sizeof(socomm_conn_handler_t));

  return handler;
}

void socomm_conn_handler_destroy(socomm_conn_handler_t **handler)
{
  free(*handler);
  *handler = NULL;
}
