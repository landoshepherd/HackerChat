//
// Created by Lando Shepherd on 12/9/23.
//

#ifndef HACKERCHAT_WEBSOCKETCLIENT_HPP
#define HACKERCHAT_WEBSOCKETCLIENT_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class WebSocketClient : public std::enable_shared_from_this<WebSocketClient> {
private:
    tcp::resolver _resolver;
    websocket::stream<beast::tcp_stream> _ws;
    boost::asio::steady_timer _timer;
    beast::flat_buffer _buffer;
    std::string _host;
    std::string _port;
    std::string _text;

public:
    // Resolver and socket require an io_context
    explicit WebSocketClient(net::io_context& _ioc, const std::string& host, const std::string& port, const std::string& text);
    ~WebSocketClient();
    void _Start();
    void run();
    void on_resolve(beast::error_code ec, const tcp::resolver::results_type& results);
    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    void on_handshake(beast::error_code ec);
    void _SendMessage(std::string& message);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_close(beast::error_code ec);
    void fail(beast::error_code ec, char const* what);
    void _KeepAlive(beast::error_code ec);
    //int Start();
};


#endif //HACKERCHAT_WEBSOCKETCLIENT_HPP
