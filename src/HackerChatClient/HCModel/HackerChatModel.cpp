//
// Created by Lando Shepherd on 9/28/24.
//

#include "HackerChatModel.hpp"
#include <iostream>

void HackerChatModel::StoreMessage(HCCommonBaseCommand& message){
    messages.push_back(message);
    std::cout << "From Model: " << message._getMessage() << std::endl;
}