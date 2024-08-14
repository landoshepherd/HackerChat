//
// Created by Lando Shepherd on 8/11/24.
//

#include "HCCommonBase.h"
#include "document.h"
#include <iostream>
#include <boost/uuid/uuid_serialize.hpp>

HCCommonBase::HCCommonBase():
    _source(),
    _destination(),
    _message(){

}

uuid HCCommonBase::_getSource(){
    return _source;
}

uuid HCCommonBase::_getDestination(){
    return _destination;
}

std::string HCCommonBase::_getMessage(){
    return _message;
}

bool HCCommonBase::_deserialize(const std::string& jsonString){
    bool rc = true;
    try {
        rapidjson::Document document;
        document.Parse(jsonString.c_str());
        boost::uuids::string_generator gen;
        _source = gen(document["_source"].GetString());
        _destination = gen(document["_destination"].GetString());
        _message = document["_message"].GetString();
    }
    catch(std::exception ex){ //TODO: Figure what throws what and catch that
        std::cout << "Failed to deserialize HCCommandBase json string. " << ex.what() << std::endl;
        rc = false;
    }

    return rc;
}