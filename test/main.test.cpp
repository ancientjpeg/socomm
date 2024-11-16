//  Hello World server

#include <socomm/socomm.h>

int main(void)
{
  socomm::broadcast_handler bh;
  bh.poll();
}
