//  Hello World server

#include <chrono>
#include <iomanip>
#include <iostream>
#include <socomm/socomm.h>
#include <sstream>
#include <unistd.h>

int main(void)
{
  std::cout << "Begin Test" << std::endl;
  socomm_broadcast_handler *bh  = socomm_broadcast_handler_create();
  socomm_broadcast_handler *bh2 = socomm_broadcast_handler_create();

  for (int i = 0; i < 10; ++i) {

    auto              now        = std::chrono::system_clock::now();
    auto              now_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << std::endl;

    auto s0 = "node0: " + ss.str();
    socomm_broadcast_handler_post(bh, (void *)s0.data(), s0.size());
    auto s1 = "node1: " + ss.str();
    socomm_broadcast_handler_post(bh, (void *)s1.data(), s1.size());

    usleep(2.5e5);
    while (socomm_broadcast_handler_poll(bh));
    usleep(2.5e5);
  }
}
