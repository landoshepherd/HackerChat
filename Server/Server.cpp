#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

#include "Server.hpp"
#include "Session.hpp"

using boost::asio::ip::tcp;

Server::Server() :
        m_errorCode(),
        m_numOfActiveSessions(0){}

Server::~Server() = default;

bool Server::InitializeServer() {
    bool status = true;
    std::string errorMessage;
    std::cout << "Initializing server..." << std::endl;
    try {
        // Set address and port
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(8000);
        m_context = std::make_shared<boost::asio::io_context>(1);
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
        status = false;
    }

    return status;
}



bool Server::Start() {
    bool status = true;
    std::string errorMessage;
    beast::flat_buffer buffer;
    std::string message;
    std::cout << "Starting server..." << std::endl;

    try
    {
        auto const address = net::ip::make_address("127.0.0.1");
        auto const port = static_cast<unsigned short>(8001);

        //net::io_context ioc(1);
        //tcp::acceptor acceptor{*m_context, {address, port}};

        // TODO: Need to create a new session for new users that join
        Session session;
        session.Initialize();

        while(true)
        {
            tcp::socket socket(*m_context);

            //NOTE: There needs to be an async version of this and the completion handler should be a function
            //that adds the socket to a session.
            m_acceptor->accept(socket);

            session.AddSocketToSession(socket);

            if (session.GetNumOfConnections() < 2){
                std::cout << "Recieved first connection." << std::endl;
            }
            else{
                std::cout << "Recieved second connection. Starting session..." << std::endl;

                std::thread sessionThread([&session](){
                    session.Start();
                });

                sessionThread.join();
            }
            //do_session(socket);
        }
    }
    catch (const std::exception& e)
    {
        errorMessage = "Exception. Location: Server.cpp - Start(). ";
        std::cerr << errorMessage << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return status;
}