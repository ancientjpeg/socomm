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
  socomm::broadcast_handler bh;
  socomm::broadcast_handler bh2;

  for (int i = 0; i < 10; ++i) {

    auto              now        = std::chrono::system_clock::now();
    auto              now_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%H:%M:%S") << std::endl;

    auto s0 = "node0: " + ss.str();
    bh.post((void *)s0.data(), s0.size());
    auto s1 = "node1: " + ss.str();
    bh2.post((void *)s1.data(), s1.size());

    usleep(2.5e5);
    while (bh.poll());
    usleep(2.5e5);
  }
}
