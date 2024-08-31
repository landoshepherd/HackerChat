//
// Created by Lando Shepherd on 8/14/24.
//
#include <iostream>
#include <string>
#include "HCCommonBaseCommand/include/HCCommonBaseCommand.h"
#include <boost/uuid/uuid_io.hpp>

int main(){
    std::string firstCommand = "{\n"
                               "  \"_source\": \"21972c83-d79d-4070-993e-8c429d0b606a\",\n"
                               "  \"_destination\": \"21972c83-d79d-4070-993e-8c429d0b606a\",\n"
                               "  \"_message\": \"First message\"\n"
                               "}";

    HCCommonBaseCommand command = HCCommonBaseCommand::_deserialize(firstCommand);
    std::cout << "Source UUID: " << command._getSource() << "\n";
    std::cout << "Destination UUID: " << command._getDestination() << "\n";
    std::cout << "Message: " << command._getMessage() << std::endl;

    std::cout << HCCommonBaseCommand::_serialize(command) << std::endl;

    return 0;
}