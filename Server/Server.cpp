#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

#include "Server.hpp"

using boost::asio::ip::tcp;

class Session{
private:
    boost::asio::io_context io_context;
    std::vector<tcp::socket> m_connections;

public:
    void AddSocketToSession(tcp::socket& socket){
        m_connections.push_back(std::move(socket));
    }

    void do_session()
    {
        try
        {
            // Construct the stream by moving in the socket
            websocket::stream<tcp::socket> ws1{std::move(m_connections[0])};
            websocket::stream<tcp::socket> ws2{std::move(m_connections[1])};

            // Set a decorator to change the Server of the handshake
            ws1.set_option(websocket::stream_base::decorator(
                    [](websocket::response_type& res)
                    {
                        res.set(http::field::server,
                                std::string(BOOST_BEAST_VERSION_STRING) +
                                " websocket-server-async");
                    }));

            ws2.set_option(websocket::stream_base::decorator(
                    [](websocket::response_type& res)
                    {
                        res.set(http::field::server,
                                std::string(BOOST_BEAST_VERSION_STRING) +
                                " websocket-server-async");
                    }));

            // Accept the websocket handshake
            ws1.async_accept(beast::bind_front_handler(shared_from_this()));
            ws2.accept();

            std::cout << "Websocket handshakes accepted." << std::endl;

            while(true)
            {
                // This buffer will hold the incoming message
                beast::flat_buffer buffer1;
                beast::flat_buffer buffer2;

                // Read a message from client 1
                ws1.read(buffer1);
                std::string incomingMessage = beast::buffers_to_string(buffer1.data());
                std::cout << "Received from client 1: " << incomingMessage << std::endl;

                // Read message from client 2
                ws2.read(buffer2);
                incomingMessage = beast::buffers_to_string(buffer2.data());
                std::cout << "Received from client 2: " << incomingMessage << std::endl;

                ws1.write(buffer2.data());
                ws2.write(buffer1.data());

                buffer1.clear();
                buffer2.clear();
                //ws2.write(net::buffer(incomingMessage));

                // Echo the message back
//            ws.text(ws.got_text());
//            ws.write(buffer.data());
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

    int GetNumOfConnections(){
        return m_connections.size();
    }
};

Server::Server() :
        m_errorCode() {}

Server::~Server() = default;

bool Server::InitializeServer() {
    bool status = true;
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

        // The io_context is required for all I/O
        net::io_context ioc(1);

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ioc, {address, port}};

        Session session;
        while(true)
        {
            // This will receive the new connection
            tcp::socket socket(ioc);

            // Block until we get a connection
            acceptor.accept(socket);
            session.AddSocketToSession(socket);

            if (session.GetNumOfConnections() == 1){
                std::cout << "Recieved first connection." << std::endl;
            }
            else if (session.GetNumOfConnections() == 2){
                std::cout << "Recieved second connection. Starting session..." << std::endl;

                //Launch the session, transferring ownership of the session object
                std::thread sessionThread([&session](){
                    session.do_session();
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