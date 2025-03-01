//
// Created by Lando Shepherd on 1/21/25.
//

#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include <queue>
#include <condition_variable>
#include <mutex>

#include "HCCommonBaseCommand.hpp"

class MessageQueue {
private:
  std::queue<HCCommonBaseCommand> _queue;
  std::mutex _mutex;

public:
  MessageQueue();
  ~MessageQueue();
  void push(const HCCommonBaseCommand& command);
  std::shared_ptr<HCCommonBaseCommand> wait_and_pop();
};



#endif //MESSAGEQUEUE_HPP
