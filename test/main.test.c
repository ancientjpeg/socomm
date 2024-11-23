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

    usleep(2.5e5);
    while (!socomm_broadcast_handler_poll(bh0));
    usleep(2.5e5);
  }

  socomm_broadcast_handler_destroy(&bh0);
  socomm_broadcast_handler_destroy(&bh1);
  assert(bh0 == NULL);
  assert(bh1 == NULL);
}
