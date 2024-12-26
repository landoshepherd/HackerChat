//
// Created by Lando Shepherd on 9/28/24.
//

#ifndef HACKERCHATCLIENT_HACKERCHATMODEL_HPP
#define HACKERCHATCLIENT_HACKERCHATMODEL_HPP

#include "HCCommonBaseCommand/HCCommonBaseCommand.h"

#include <vector>

class HackerChatModel {
private:
    std::vector<HCCommonBaseCommand> messages;

public:
    HackerChatModel() = default;
    ~HackerChatModel() = default;
    void StoreMessage(HCCommonBaseCommand& message);
};


#endif //HACKERCHATCLIENT_HACKERCHATMODEL_HPP
