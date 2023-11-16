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
#include <mutex>
#include <queue>

using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;
namespace net = boost::asio;

class Session : public std::enable_shared_from_this<Session> {
private:
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    std::queue<std::string> m_messageQueue;
    std::mutex m_queueLock;

private:
    void OnRun();
    void OnAccept(beast::error_code ec);
    void DoRead();
    void OnRead(beast::error_code ec, std::size_t bytes_transferred);
    void OnWrite(beast::error_code ec, std::size_t bytes_transferred);
    void fail(beast::error_code ec, char const* what);

public:
    explicit Session(tcp::socket&& socket);
    void Run();
    ~Session() = default;
};

#endif //HACKERCHAT_SESSION_HPP
