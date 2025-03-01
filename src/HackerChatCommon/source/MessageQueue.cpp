//
// Created by Lando Shepherd on 1/21/25.
//

#include "../include/MessageQueue.hpp"
#include "../include/HCCommonBaseCommand.hpp"

void MessageQueue::push(const HCCommonBaseCommand& command) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(command);
}

std::shared_ptr<HCCommonBaseCommand> MessageQueue::wait_and_pop(){
    std::unique_lock<std::mutex> lock(_mutex);

    // Get to top message in the queue
    std::shared_ptr<HCCommonBaseCommand> result(std::make_shared<HCCommonBaseCommand>(_queue.front()));

    // Remove the top command from the queue
    _queue.pop();

    return result;
}