//  Hello World server

#include "socomm/broadcast_handler.h"
#include "test_helpers.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int handler_post_time(socomm_broadcast_handler *bh,
                      socomm_header             header,
                      int                       idx)
{
#define max_strlen 64
  char            time_str[max_strlen]    = {};
  char            message_str[max_strlen] = {};

  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  int us = ts.tv_nsec / 1e03;

  strftime(time_str, max_strlen, "%H:%M:%S", gmtime(&ts.tv_sec));

  snprintf(message_str,
           max_strlen,
           "Handler %d broadcasts at time: %s:%07d\n",
           idx,
           time_str,
           us);

  socomm_message *msg = socomm_message_create(header,
                                              SOCOMM_MESSAGE_TYPES[0],
                                              message_str,
                                              strlen(message_str));
  return socomm_broadcast_handler_post(bh, msg, strlen(message_str));
}

int poll_handler(socomm_broadcast_handler *bh, int idx, int timeout_ms)
{
  socomm_message *message = NULL;
  message = socomm_broadcast_handler_poll_blocking(bh, timeout_ms);
  if (message != NULL) {

    const char *data = socomm_message_data_size(message) == 0
                           ? socomm_message_data(message)
                           : "Message empty.";
    debug_printf("Handler %d received message: %s\n", idx, data);
  }
  socomm_message_destroy(&message);
  return message != NULL ? 0 : 1;
}

int main(void)
{
  const char *gname = "test_gname";
  debug_printf("Begin Test\n");
  socomm_broadcast_handler *bh0 = socomm_broadcast_handler_create(gname);
  socomm_broadcast_handler *bh1 = socomm_broadcast_handler_create(gname);

  for (int i = 0; i < 10; ++i) {

    debug_printf("ITERATION %d:\n", i);

    socomm_header header0 = get_test_header();
    socomm_header header1 = get_test_header();

    handler_post_time(bh0, header0, 0);
    handler_post_time(bh1, header1, 1);

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

  return 0;
}
