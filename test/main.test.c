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

#define max_strlen 64
    char       time_str[max_strlen] = {};
    char       s0[max_strlen]       = {};
    char       s1[max_strlen]       = {};

    time_t     now_time_t           = time(NULL);
    struct tm *now                  = localtime(&now_time_t);
    strftime(time_str, max_strlen, "%H:%M:%S", now);

    snprintf(s0, max_strlen, "BUFFER0: %s\n", time_str);
    snprintf(s1, max_strlen, "BUFFER1: %s\n", time_str);

    printf("s0 strlen%lu\n", strlen(s0));
    socomm_broadcast_handler_post(bh0, (void *)s0, strlen(s0));
    socomm_broadcast_handler_post(bh1, (void *)s1, strlen(s1));

    int rc0, rc1;
    do {
      socomm_string *buf;
      rc0 = socomm_broadcast_handler_poll(bh0, &buf);

      assert(rc0 != -1 || errno == EAGAIN);

      if (!rc0) {
        printf("Handler 0 received message: %s\n",
               (const char *)socomm_string_data(buf));
      }

      rc1 = socomm_broadcast_handler_poll(bh1, &buf);
      assert(rc1 != -1 || errno == EAGAIN);
      if (!rc1) {
        printf("Handler 1 received message: %s\n",
               (const char *)socomm_string_data(buf));
      }

      printf("sleep 50\n");
      usleep(50);
    } while (!rc0 && !rc1);

    usleep(2.5e5);
  }

  socomm_broadcast_handler_destroy(&bh0);
  socomm_broadcast_handler_destroy(&bh1);
  assert(bh0 == NULL);
  assert(bh1 == NULL);
}
