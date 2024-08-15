//
// Created by Lando Shepherd on 8/11/24.
//

#include "HCCommonBaseCommand.h"
#include "document.h"
#include <iostream>
#include <boost/uuid/string_generator.hpp>


//TODO: Provide default values
HCCommonBaseCommand::HCCommonBaseCommand():
        m_source(),
        m_destination(),
        m_message(){
}

HCCommonBaseCommand::HCCommonBaseCommand(const uuid p_source,
                                         const uuid p_destination,
                                         const std::string p_message):
        m_source(p_source),
        m_destination(p_destination),
        m_message(p_message){}

uuid HCCommonBaseCommand::m_getSource(){
    return m_source;
}

uuid HCCommonBaseCommand::m_getDestination(){
    return m_destination;
}

std::string HCCommonBaseCommand::m_getMessage(){
    return m_message;
}

void HCCommonBaseCommand::m_setSource(const uuid p_source) {
    m_source = p_source;
}

void HCCommonBaseCommand::m_setDestination(const uuid p_destination) {
    m_destination = p_destination;
}

void HCCommonBaseCommand::m_setMessage(const std::string& p_message) {
    m_message = p_message;
}

HCCommonBaseCommand HCCommonBaseCommand::m_deserialize(const std::string& jsonString){
    HCCommonBaseCommand command;

    try {
        rapidjson::Document document;
        document.Parse(jsonString.c_str());
        boost::uuids::string_generator gen;
        uuid source = gen(std::string(document["m_source"].GetString()));
        uuid destination = gen(std::string(document["m_destination"].GetString()));
        std::string message = document["m_message"].GetString();
        command.m_setSource(source);
        command.m_setDestination(destination);
        command.m_setMessage(message);
    }
    catch(std::exception ex){ //TODO: Figure what throws what and catch that
        std::cout << "Failed to deserialize HCCommandBase json string. " << ex.what() << std::endl;
    }

    return command;
}