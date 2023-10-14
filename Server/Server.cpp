#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

#include "Server.hpp"

using boost::asio::ip::tcp;


Server::Server() :
        m_errorCode() {}

Server::~Server() = default;

bool Server::InitializeServer() {
    bool rc = true;
    std::string errorMessage;
    std::cout << "Initializing server..." << std::endl;
    try {
        // Set address and port
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(8000);
        m_context = std::make_shared<boost::asio::io_context>();
        m_acceptor = std::make_shared<tcp::acceptor>(*m_context, boost::asio::ip::tcp::endpoint(address, port));
        m_socket = std::make_shared<tcp::socket>(*m_context);
        m_webSocketStream = std::make_shared<websocket::stream<tcp::socket>>(*m_context);

        // Set a decorator to change the Server of the handshake
        m_webSocketStream->set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res)
                {
                    res.set(http::field::server,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-sync");
                }));

    }
    catch (std::exception& e) {
        errorMessage = "Exception. Failed to initialize server.";
        std::cout << errorMessage << std::endl;
        std::cout << e.what() << std::endl;
        rc = false;
    }

    return rc;
}

void Server::do_session(tcp::socket& socket)
{
    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{std::move(socket)};

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
                [](websocket::response_type& res)
                {
                    res.set(http::field::server,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-server-sync");
                }));

        // Accept the websocket handshake
        ws.accept();

        std::cout << "Websocket handshake accepted." << std::endl;

        for(;;)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message
            ws.read(buffer);

            std::string incomingMessage = beast::buffers_to_string(buffer.data());

            std::cout << incomingMessage << std::endl;

            // Echo the message back
            ws.text(ws.got_text());
            ws.write(buffer.data());
        }
    }
    catch(beast::system_error const& se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

bool Server::Start() {
    bool rc = true;
    std::string errorMessage;
    beast::flat_buffer buffer;
    std::string message;
    std::cout << "Starting server..." << std::endl;

    try
    {
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(8001);

        // The io_context is required for all I/O
        net::io_context ioc(1);

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};
        while(true)
        {
            // This will receive the new connection
            tcp::socket socket(ioc);

            // Block until we get a connection
            acceptor.accept(socket);

            std::cout << "Socket connection accepted." << std::endl;
            // Launch the session, transferring ownership of the socket
//            std::thread sessionThread([this, &socket](){
//                this->do_session(socket);
//            });
//
//            sessionThread.join();
            do_session(socket);
        }
    }
    catch (const std::exception& e)
    {
        errorMessage = "Exception. Location: Server.cpp - Start(). ";
        std::cerr << errorMessage << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return rc;
}