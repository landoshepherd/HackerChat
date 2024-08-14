//
// Created by Lando Shepherd on 8/11/24.
//

#ifndef HACKERCHATCOMMON_HCCOMMONBASE_H
#define HACKERCHATCOMMON_HCCOMMONBASE_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>

using namespace boost::uuids;

class HCCommonBase {
private:
    uuid _source;
    uuid _destination;
    std::string _message;
public:
    HCCommonBase();
    ~HCCommonBase() = default;
    uuid _getSource();
    uuid _getDestination();
    std::string _getMessage();
    bool _deserialize(const std::string& jsonString);
};


#endif //HACKERCHATCOMMON_HCCOMMONBASE_H
