//  Hello World server

#include "socomm/broadcast_handler.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifndef SOCOMM_VERBOSE_TESTS
#define debug_printf(...)
#else
#define debug_printf printf
#endif

int poll_handler(socomm_broadcast_handler *bh, int idx, int timeout_ms)
{
  socomm_string *str = NULL;
  int rc = socomm_broadcast_handler_poll_blocking(bh, &str, timeout_ms);
  if (rc >= 0) {
    debug_printf("Handler %d received message: %s\n",
                 idx,
                 (const char *)socomm_string_data(str));
  }
  socomm_string_destroy(&str);
  return rc;
}

int main(void)
{
  const char *gname = "test_gname";
  debug_printf("Begin Test\n");
  socomm_broadcast_handler *bh0 = socomm_broadcast_handler_create(gname);
  socomm_broadcast_handler *bh1 = socomm_broadcast_handler_create(gname);

  for (int i = 0; i < 10; ++i) {

    debug_printf("ITERATION %d:\n", i);
#define max_strlen 64
    char            time_str[max_strlen] = {};
    char            s0[max_strlen]       = {};
    char            s1[max_strlen]       = {};

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    int us = ts.tv_nsec / 1e03;

    strftime(time_str, max_strlen, "%H:%M:%S", gmtime(&ts.tv_sec));

    snprintf(s0, max_strlen, "Handler0 broadcasts: %s:%07d\n", time_str, us);
    socomm_broadcast_handler_post(bh0, (void *)s0, strlen(s0));

    snprintf(s1, max_strlen, "Handler1 broadcasts: %s:%07d\n", time_str, us);
    socomm_broadcast_handler_post(bh1, (void *)s1, strlen(s1));

    int timeout = 1;
    assert(poll_handler(bh0, 0, timeout) >= 0);
    assert(poll_handler(bh0, 0, timeout) >= 0);
    assert(poll_handler(bh0, 0, 0) == -1 && errno == EAGAIN);
    assert(poll_handler(bh1, 1, timeout) >= 0);
    assert(poll_handler(bh1, 1, timeout) >= 0);
    assert(poll_handler(bh1, 1, 0) == -1 && errno == EAGAIN);
  }

  socomm_broadcast_handler_destroy(&bh0);
  socomm_broadcast_handler_destroy(&bh1);
  assert(bh0 == NULL);
  assert(bh1 == NULL);
}
