//
// Created by Lando Shepherd on 10/14/23.
//
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Client.hpp"
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

Client::Client()
        : m_buffer(),
          m_error() {}

Client::~Client() {}

bool Client::InitializeClient(std::string &serverName, std::string &serviceName) {
    bool rc = true;
    if (!started) {
        try {
            std::cout << "Initializing client..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));

            m_context.reset(new boost::asio::io_context());
            m_tcpResolver.reset(new tcp::resolver(*m_context));
            m_socket.reset(new tcp::socket(*m_context));

            if (!ResolveEndPoints(serviceName)) {
                return false;
            }

            if (!ConnectToSocket()) {
                return false;
            }

            started = true;
        }
        catch (std::exception& ex) {
            std::cout << "Exception. Location: Daytime1.cpp - InitializeClient(). What: Failed to initialize client: "
                      << serviceName << std::endl;
            rc = false;
            return rc;
        }
    }
    std::cout << "Client initialized..." << std::endl;
    return rc;
}

bool Client::Start() {
    try
    {
        std::string host = "127.0.0.1";
        auto const  port = "8001";
        auto const  text = "Hello";

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        auto ep = net::connect(ws.next_layer(), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        host += ':' + std::to_string(ep.port());

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
                [](websocket::request_type& req)
                {
                    req.set(http::field::user_agent,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
                }));

        // Perform the websocket handshake
        ws.handshake(host, "/");

        while(true){
            std::string message;
            std::getline(std::cin, message);

            // Send the message
            ws.write(net::buffer(message));
        }

        // This buffer will hold the incoming message
        beast::flat_buffer buffer;

        // Read a message into our buffer
        ws.read(buffer);

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << beast::make_printable(buffer.data()) << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool Client::ResolveEndPoints(std::string &serviceName) {
    bool rc = true;

    try {
        m_endPoints = m_tcpResolver->resolve("127.0.0.1", serviceName);
        if (m_endPoints.empty()) {
            std::cout << "Could not resolve server name" << std::endl;
            rc = false;
        }
    }
    catch (...) {
        rc = false;
        std::cout << "Exception. Location: Daytime1.cpp - m_ResolveEndPoints(). What: Failed to resolve endpoints."
                  << std::endl;
    }
    return rc;
}

bool Client::ConnectToSocket() {
    bool rc = false;
    boost::system::error_code errorCode;

    try {
        boost::asio::connect(*m_socket, m_endPoints, errorCode);

        if (!m_socket->is_open()) {
            std::cout << "Socket failed to connect." << std::endl;
            return false;
        } else {
            std::cout << "Socket is open." << std::endl;
            return true;
        }
    }
    catch (std::exception& e) {
        rc = false;
        std::cout << "Exception. Location: Daytime1.cpp - m_ConnectToSocket(). Failed to connect to socket."
                  << std::endl;
        std::cout << "Error Code: " << errorCode.message() << std::endl;
    }

    return rc;
}
