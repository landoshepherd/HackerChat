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
namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;
namespace net = boost::asio;

struct Connection {
    std::unique_ptr<tcp::socket> connectionSocket;
    std::shared_ptr<websocket::stream<beast::tcp_stream>> connectionStream;
};

class Session : public std::enable_shared_from_this<Session> {
private:
    std::shared_ptr<boost::asio::io_context> m_io_context;
    std::vector<Connection> m_connections;
    bool m_initialized;

public:
    Session();
    void Initialize();
    bool AddSocketToSession(tcp::socket &socket);
    void DoSession(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                   std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream);
    void DoRun();
    int GetNumOfConnections();
    void DoRead(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream);
    void DoWrite(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream,
                 beast::flat_buffer buffer1);
    void ClearBuffer(beast::flat_buffer buffer1);
    void Start();
    ~Session();
};

#endif //HACKERCHAT_SESSION_HPP
