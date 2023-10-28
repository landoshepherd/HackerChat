//
// Created by Lando Shepherd on 10/25/23.
//

#ifndef HACKERCHAT_SESSION_HPP
#define HACKERCHAT_SESSION_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <vector>

using boost::asio::ip::tcp;
class Session {
private:
    boost::asio::io_context io_context;
    std::vector<tcp::socket> m_connections;

public:
    Session();
    void AddSocketToSession(tcp::socket& socket);
    void do_session();
    int GetNumOfConnections();
    ~Session();
};


#endif //HACKERCHAT_SESSION_HPP
