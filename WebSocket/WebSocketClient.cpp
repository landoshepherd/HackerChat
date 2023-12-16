//
// Created by Lando Shepherd on 12/9/23.
//

#include "WebSocketClient.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "WebSocketClient.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Resolver and socket require an io_context
WebSocketClient::WebSocketClient(const std::string& host, const std::string& port, const std::string& text):
        _ioc(1),
        _resolver (net::make_strand(_ioc)),
        _ws(net::make_strand(_ioc)),
        _buffer(),
        _host(host),
        _port(port),
        _text(text){}

WebSocketClient::~WebSocketClient() {
}

void WebSocketClient::_Start(){
    run();
    _ioc.run();
}

// Start the asynchronous operation
void WebSocketClient::run() {
    // Look up the domain name
    _resolver.async_resolve(_host, _port, beast::bind_front_handler(&WebSocketClient::on_resolve, shared_from_this()));
}

void WebSocketClient::on_resolve(beast::error_code ec, const tcp::resolver::results_type& results) {
    if (ec) {
        return fail(ec, "resolve");
    }

    // Set the timeout for the operation
    beast::get_lowest_layer(_ws).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(_ws).async_connect(results, beast::bind_front_handler(&WebSocketClient::on_connect, shared_from_this()));
}

void WebSocketClient::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
    if (ec) {
        return fail(ec, "connect");
    }

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(_ws).expires_never();

    // Set suggested timeout settings for the websocket
    _ws.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    _ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type &req) {
                req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-async");
            }));

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    _host += ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    _ws.async_handshake(_host, "/", beast::bind_front_handler(&WebSocketClient::on_handshake, shared_from_this()));
}

void WebSocketClient::on_handshake(beast::error_code ec) {
    if (ec) {
        return fail(ec, "handshake");
    }

    // Send the message
    _ws.async_write(net::buffer(_text), beast::bind_front_handler(&WebSocketClient::on_write, shared_from_this()));
}

void WebSocketClient::_SendMessage(std::string& message){
    _text = message;
    _ws.async_write(net::buffer(_text), beast::bind_front_handler(&WebSocketClient::on_write, shared_from_this()));
}

void WebSocketClient::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        return fail(ec, "write");
    }

    // Read a message into our buffer
    _ws.async_read(_buffer, beast::bind_front_handler(&WebSocketClient::on_read, shared_from_this()));
}

void WebSocketClient::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        return fail(ec, "read");
    }

    // The make_printable() function helps print a ConstBufferSequence
    std::cout << beast::make_printable(_buffer.data()) << std::endl;

    _buffer.clear();

    std::string message = "Take this";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    _SendMessage(message);
    // Close the WebSocket connection
    //_ws.async_close(websocket::close_code::normal, beast::bind_front_handler(&WebSocketClient::_SendMessage, message, shared_from_this()));
}

void WebSocketClient::on_close(beast::error_code ec) {
    if (ec) {
        return fail(ec, "close");
    }

    // If we get here then the connection is closed gracefully
}

void WebSocketClient::fail(beast::error_code ec, char const* what){
    std::cout << what << ": " << ec.message() << "\n";
}