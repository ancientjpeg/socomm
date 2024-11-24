//  Hello World server

#include <assert.h>
#include <socomm/socomm.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
  printf("Begin Test\n");
  socomm_broadcast_handler *bh0 = socomm_broadcast_handler_create();
  socomm_broadcast_handler *bh1 = socomm_broadcast_handler_create();

  for (int i = 0; i < 10; ++i) {

    printf("ITERATION %d:\n", i);
#define max_strlen 64
    char            time_str[max_strlen] = {};
    char            s0[max_strlen]       = {};
    char            s1[max_strlen]       = {};

    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    int ms = ts.tv_nsec / 10e6;

    strftime(time_str, max_strlen, "%H:%M:%S", gmtime(&ts.tv_sec));

    snprintf(s0, max_strlen, "Handler0 broadcasts: %s:%d\n", time_str, ms);
    socomm_broadcast_handler_post(bh0, (void *)s0, strlen(s0));

    snprintf(s1, max_strlen, "Handler1 broadcasts: %s:%d\n", time_str, ms);
    socomm_broadcast_handler_post(bh1, (void *)s1, strlen(s1));

    int  rc0 = -1, rc1 = -1;
    bool success = false;
    for (int tries = 0; tries < 10; ++tries) {

      socomm_string *str = NULL;
      if (rc0 < 0) {
        rc0 = socomm_broadcast_handler_poll(bh0, &str);

        assert(rc0 != -1 || errno == EAGAIN);

        if (rc0 >= 0) {
          printf("Handler 0 received message: %s\n",
                 (const char *)socomm_string_data(str));
        }
      }

      if (rc1 < 0) {
        rc1 = socomm_broadcast_handler_poll(bh1, &str);
        assert(rc1 != -1 || errno == EAGAIN);

        if (rc1 >= 0) {
          printf("Handler 1 received message: %s\n",
                 (const char *)socomm_string_data(str));
        }
      }

      /* printf("sleep 50\n"); */
      usleep(100);
      socomm_string_destroy(&str);

      if (rc0 >= 0 && rc1 >= 0) {
        success = true;
        break;
      }
    }

    assert(success);

    usleep(2.5e5);
  }

  socomm_broadcast_handler_destroy(&bh0);
  socomm_broadcast_handler_destroy(&bh1);
  assert(bh0 == NULL);
  assert(bh1 == NULL);
}
