/**
 * @file broadcast_handler.h
 * @author Jackson Kaplan (JwyattK@gmail.com)
 * @date 2024-11-15
 * Copyright (c) 2024. All rights reserved.
 */

#ifndef SOCOMM_BROADCAST_HANDLER_H_
#define SOCOMM_BROADCAST_HANDLER_H_

namespace socomm {
class broadcast_handler {
public:
  broadcast_handler();

  ~broadcast_handler();

  void event_loop();

private:
};
} // namespace socomm

#endif
