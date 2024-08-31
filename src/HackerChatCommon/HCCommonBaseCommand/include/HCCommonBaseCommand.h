//
// Created by Lando Shepherd on 8/11/24.
//

#ifndef HACKERCHATCOMMON_HCCOMMONBASECOMMAND_H
#define HACKERCHATCOMMON_HCCOMMONBASECOMMAND_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>

using namespace boost::uuids;

class HCCommonBaseCommand {
public:
    HCCommonBaseCommand();
    HCCommonBaseCommand(uuid p_source, uuid p_destination, std::string p_message);
    ~HCCommonBaseCommand() = default;
    uuid _getSource();
    uuid _getDestination();
    std::string _getMessage();
    void _setSource(uuid p_source);
    void _setDestination(uuid p_destination);
    void _setMessage(const std::string& p_message);
    static HCCommonBaseCommand _deserialize(const std::string& jsonString);
    static std::string _serialize(HCCommonBaseCommand& command);

private:
    uuid _source;
    uuid _destination;
    std::string _message;
};


#endif //HACKERCHATCOMMON_HCCOMMONBASECOMMAND_H
