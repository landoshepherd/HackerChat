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
private:
    uuid m_source;
    uuid m_destination;
    std::string m_message;
public:
    HCCommonBaseCommand();
    HCCommonBaseCommand(const uuid p_source, const uuid p_destination, const std::string p_message);
    ~HCCommonBaseCommand() = default;
    uuid m_getSource();
    uuid m_getDestination();
    std::string m_getMessage();
    void m_setSource(const uuid p_source);
    void m_setDestination(const uuid p_destination);
    void m_setMessage(const std::string& p_message);

    HCCommonBaseCommand static m_deserialize(const std::string& jsonString);
};


#endif //HACKERCHATCOMMON_HCCOMMONBASECOMMAND_H
