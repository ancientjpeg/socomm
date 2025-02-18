//  Hello World server

#include "socomm/broadcast_handler.h"
#include "test_helpers.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define max_strlen 64

void log_message(int idx, socomm_message *message)
{
  const char  *data = socomm_message_data(message);
  const size_t size = socomm_message_data_size(message);

  printf("Handler %d receives message: %s\n", idx, data);
}

void gen_message(char *message_str, int handler_idx)
{

  char            time_str[max_strlen] = {};

  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  int us = ts.tv_nsec / 1e03;

  strftime(time_str, max_strlen, "%H:%M:%S", gmtime(&ts.tv_sec));

  snprintf(message_str,
           max_strlen,
           "Handler %d broadcasts at time: %s:%07d\n",
           handler_idx,
           time_str,
           us);
}

int handler_post_string(socomm_broadcast_handler *bh,
                        socomm_header             header,
                        int                       idx,
                        const char               *message_str)
{

  socomm_message *msg        = socomm_message_create(header,
                                              SOCOMM_MESSAGE_TYPES[0],
                                              message_str,
                                              strlen(message_str));

  int             bytes_sent = socomm_broadcast_handler_post(bh, msg);
  socomm_message_destroy(&msg);
  return bytes_sent;
}

socomm_message *
poll_handler(socomm_broadcast_handler *bh, int idx, int timeout_ms)
{
  socomm_message *message = NULL;
  message = socomm_broadcast_handler_poll_blocking(bh, timeout_ms);

#ifdef SOCOMM_VERBOSE_TESTS
  if (message != NULL) {
    log_message(idx, message);
  }
#endif

  return message;
}

int main(void)
{
  socomm_header             header0 = get_test_header();
  socomm_header             header1 = get_test_header();

  socomm_broadcast_handler *bh0     = socomm_broadcast_handler_create(header0);
  socomm_broadcast_handler *bh1     = socomm_broadcast_handler_create(header1);

  char                      message_str_0[max_strlen] = {};
  char                      message_str_1[max_strlen] = {};

  for (int i = 0; i < 10; ++i) {

    debug_printf("ITERATION %d:\n", i);

    /* each iteration begins with empty queue */
    assert(poll_handler(bh0, 0, 0) == NULL && errno == EAGAIN);
    assert(poll_handler(bh1, 1, 0) == NULL && errno == EAGAIN);

    gen_message(message_str_0, 0);
    gen_message(message_str_1, 1);

    int send_code_0
        = handler_post_string(bh0, header0, 0, (const char *)message_str_0);
    int send_code_1
        = handler_post_string(bh1, header1, 1, (const char *)message_str_1);
    assert(send_code_0 >= 0);
    assert(send_code_1 >= 0);

    int             timeout    = 1000;
    socomm_message *bh0_recv_0 = poll_handler(bh0, 0, timeout);

    assert(bh0_recv_0 != NULL);

    assert(poll_handler(bh0, 0, 0) == NULL && errno == EAGAIN);

    socomm_message *bh1_recv_0 = poll_handler(bh1, 1, timeout);

    assert(bh1_recv_0 != NULL);

    assert(poll_handler(bh1, 1, 0) == NULL && errno == EAGAIN);
  }

  int     timeout_test_ms = 50;
  clock_t start           = clock();
  poll_handler(bh0, 0, timeout_test_ms);
  clock_t end = clock();

  assert(((double)(end - start) / CLOCKS_PER_SEC * 1e03) >= timeout_test_ms);

  socomm_broadcast_handler_destroy(&bh0);
  socomm_broadcast_handler_destroy(&bh1);
  assert(bh0 == NULL);
  assert(bh1 == NULL);

  return 0;
}
