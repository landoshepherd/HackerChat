//
// Created by Lando Shepherd on 8/11/24.
//

#include "../include/HCCommonBaseCommand.h"
#include "document.h"
#include <iostream>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <string>
#include <sstream>
#include <stringbuffer.h>
#include <prettywriter.h>


//TODO: Provide default values
HCCommonBaseCommand::HCCommonBaseCommand():
        _source(),
        _destination(),
        _message(){
}

HCCommonBaseCommand::HCCommonBaseCommand(const uuid p_source,
                                         const uuid p_destination,
                                         const std::string p_message):
        _source(p_source),
        _destination(p_destination),
        _message(p_message){}

uuid HCCommonBaseCommand::_getSource(){
    return _source;
}

uuid HCCommonBaseCommand::_getDestination(){
    return _destination;
}

std::string HCCommonBaseCommand::_getMessage(){
    return _message;
}

void HCCommonBaseCommand::_setSource(const uuid p_source) {
    _source = p_source;
}

void HCCommonBaseCommand::_setDestination(const uuid p_destination) {
    _destination = p_destination;
}

void HCCommonBaseCommand::_setMessage(const std::string& p_message) {
    _message = p_message;
}

HCCommonBaseCommand HCCommonBaseCommand::_deserialize(const std::string& jsonString){
    HCCommonBaseCommand command;
    rapidjson::Document document;
    document.Parse(jsonString.c_str());
    boost::uuids::string_generator gen;
    uuid source = gen(std::string(document["_source"].GetString()));
    uuid destination = gen(std::string(document["_destination"].GetString()));
    std::string message = document["_message"].GetString();
    command._setSource(source);
    command._setDestination(destination);
    command._setMessage(message);
    return command;
}

std::string HCCommonBaseCommand::_serialize(HCCommonBaseCommand& command){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Value value(rapidjson::kStringType);

    // Set the source
    std::string sourceUUID = boost::uuids::to_string(command._getSource());
    value.SetString(sourceUUID.c_str(), sourceUUID.length());
    document.AddMember("_source", value, document.GetAllocator());

    // Set the destination
    std::string destinationUUID = boost::uuids::to_string(command._getDestination());
    value.SetString(destinationUUID.c_str(), destinationUUID.length());
    document.AddMember("_destination", value, document.GetAllocator());

    // Set the message
    std::string message = command._getMessage();
    value.SetString(message.c_str(), message.length());
    document.AddMember("_message", value, document.GetAllocator());

    // Stringify the json document
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();

    return jsonString;
}