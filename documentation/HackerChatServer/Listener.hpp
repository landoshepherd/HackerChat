//
// Created by Lando Shepherd on 11/10/23.
//

#ifndef HACKERCHAT_LISTENER_HPP
#define HACKERCHAT_LISTENER_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/uuid/uuid.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <map>

#include "Connection.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Listener : public std::enable_shared_from_this<Listener> {

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint);
    void Run();
    void DoAccept();
    void OnAccept(beast::error_code ec, tcp::socket socket);
private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::map<std::string, std::shared_ptr<Connection>> activeConnections_;
};


#endif //HACKERCHAT_LISTENER_HPP
