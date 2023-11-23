#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "Client.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Resolver and socket require an io_context
Client::Client(net::io_context& ioc)
:resolver_ (net::make_strand(ioc))
, ws_(net::make_strand(ioc)) {}

// Start the asynchronous operation
void Client::run(char const *host, char const *port, char const *text) {
    // Save these for later
    host_ = host;
    text_ = text;

    // Look up the domain name
    resolver_.async_resolve(host, port, beast::bind_front_handler(&Client::on_resolve, shared_from_this()));
}

void Client::on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) {
        return fail(ec, "resolve");
    }

    // Set the timeout for the operation
    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(ws_).async_connect(results, beast::bind_front_handler(&Client::on_connect, shared_from_this()));
}

void Client::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
    if (ec) {
        return fail(ec, "connect");
    }

    // Turn off the timeout on the tcp_stream, because
    // the websocket stream has its own timeout system.
    beast::get_lowest_layer(ws_).expires_never();

    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(websocket::stream_base::decorator(
            [](websocket::request_type &req) {
                req.set(http::field::user_agent,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-async");
            }));

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    host_ += ':' + std::to_string(ep.port());

    // Perform the websocket handshake
    ws_.async_handshake(host_, "/", beast::bind_front_handler(&Client::on_handshake, shared_from_this()));
}

void Client::on_handshake(beast::error_code ec) {
    if (ec) {
        return fail(ec, "handshake");
    }

    // Send the message
    ws_.async_write(net::buffer(text_), beast::bind_front_handler(&Client::on_write, shared_from_this()));
}

void Client::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        return fail(ec, "write");
    }

    // Read a message into our buffer
    ws_.async_read(buffer_, beast::bind_front_handler(&Client::on_read, shared_from_this()));
}

void Client::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        return fail(ec, "read");
    }

    // Close the WebSocket connection
    ws_.async_close(websocket::close_code::normal, beast::bind_front_handler(&Client::on_close, shared_from_this()));
}

void Client::on_close(beast::error_code ec) {
    if (ec) {
        return fail(ec, "close");
    }

    // If we get here then the connection is closed gracefully

    // The make_printable() function helps print a ConstBufferSequence
    std::cout << beast::make_printable(buffer_.data()) << std::endl;
}

void Client::fail(beast::error_code ec, char const* what){
    std::cout << what << ": " << ec.message() << "\n";
}

int Client::Start() {
    auto const host = "127.0.01";
    auto const port = "8001";
    auto const text = "What up!";

    // The io_context is required for all I/O
    net::io_context ioc;

    // Launch the asynchronous operation
    std::make_shared<Client>(ioc)->run(host, port, text);

    // Run the I/O service. The call will return when
    // the socket is closed.
    ioc.run();

    return EXIT_SUCCESS;
}

////
//// Created by Lando Shepherd on 10/14/23.
////
//#include "Client.hpp"
//
//Client::Client()
//        : m_buffer(),
//          m_error(),
//          m_destinationHost(),
//          m_destinationPort(){}
//
//Client::~Client() = default;
//
//bool Client::Initialize() {
//    bool status = true;
//    std::string errorMessage;
//
//    try {
//        m_ioContext = std::make_shared<boost::asio::io_context>();
//        m_Resolver = std::make_shared<tcp::resolver>(*m_ioContext);
//        m_WebSocketStream = std::make_shared<websocket::stream<tcp::socket>>(*m_ioContext);
//    }
//    catch(std::exception& ex){
//        status = false;
//        errorMessage = "Exception. Location: Client.cpp - Initialize().\n";
//        errorMessage += ex.what();
//        std::cout << errorMessage << std::endl;
//    }
//    return status;
//}
//
//bool Client::Start() {
//    try
//    {
//        std::cout << "Starting client..." << std::endl;
//        m_destinationHost = "127.0.0.1";
//        m_destinationPort = "8001";
//        pugi::xml_document doc;
//
//        pugi::xml_node childNode = doc.append_child("test");
//        childNode.set_value("bucko");
//
//
//        if (ConnectToServer()){
//            while(true){
//                std::string message;
//                std::getline(std::cin, message);
//
//                // Send the message
//                m_WebSocketStream->write(net::buffer(message));
//
//                // This buffer will hold the incoming message
//                beast::flat_buffer buffer;
//
//                // Read a message into our buffer
//                m_WebSocketStream->read(buffer);
//                std::string incomingMessage = beast::buffers_to_string(buffer.data());
//                std::cout << incomingMessage << std::endl;
//            }
//
//
//
//
//
//            // Close the WebSocket connection
//            m_WebSocketStream->close(websocket::close_code::normal);
//
//            // If we get here then the connection is closed gracefully
//
//            // The make_printable() function helps print a ConstBufferSequence
//            //std::cout << beast::make_printable(buffer.data()) << std::endl;
//        }
//    }
//    catch(std::exception const& e)
//    {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return EXIT_FAILURE;
//    }
//    return EXIT_SUCCESS;
//}
//
//bool Client::ConnectToServer() {
//    bool status = true;
//    boost::system::error_code errorCode;
//
//    try{
//        std::cout << "Connecting to server..." << std::endl;
//        // Look up the domain name
//        auto const results = m_Resolver->resolve(m_destinationHost, m_destinationPort);
//
//        // Make the connection on the IP address we get from a lookup
//        auto endpoint = net::connect(m_WebSocketStream->next_layer(), results);
//        m_destinationHost += ':' + std::to_string(endpoint.port());
//
//        // Set a decorator to change the User-Agent of the handshake
//        m_WebSocketStream->set_option(websocket::stream_base::decorator(
//                [](websocket::request_type& req)
//                {
//                    req.set(http::field::user_agent,
//                            std::string(BOOST_BEAST_VERSION_STRING) +
//                            " websocket-client-coro");
//                }));
//
//        // Perform the websocket handshake
//        m_WebSocketStream->handshake(m_destinationHost, "/");
//
//        std::cout << "Successfully connected to server." << std::endl;
//    }
//    catch(std::exception& ex){
//        status = false;
//    }
//
//    return status;
//}
