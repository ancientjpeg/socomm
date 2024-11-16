//  Hello World server

#include <socomm/socomm.h>

int main(void)
{
  errno = 0;
  socomm::broadcast_handler bh;
  bh.poll();
}
