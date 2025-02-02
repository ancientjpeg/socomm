/**
 * @file conn_handler.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-12-30
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef CONN_HANDLER_H_
#define CONN_HANDLER_H_

typedef struct socomm_conn_handler_t socomm_conn_handler_t;

socomm_conn_handler_t               *socomm_conn_handler_create();
void socomm_conn_handler_destroy(socomm_conn_handler_t **handler);

#endif
