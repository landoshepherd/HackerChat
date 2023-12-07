#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <thread>

#include "Server.hpp"
#include "Session.hpp"
#include "Listener.hpp"

using boost::asio::ip::tcp;

Server::Server() :
        _ioc(1),
        m_errorCode(),
        m_numOfActiveSessions(0){}

Server::~Server() = default;

void Server::fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

void Server::StartServerListener(){
    beast::error_code ec;

    // Open the acceptor
    if (!_acceptor->is_open()) {
        _acceptor->open(_endPoint.protocol(), ec);

        if (ec) {
            fail(ec, "open");
            return;
        }
    }

    // Allow address reuse
    _acceptor->set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    _acceptor->bind(_endPoint, ec);
    if (ec) {
        std::cout << ec.message() << std::endl;
        fail(ec, "bind");
        return;
    }

    // Start listening for connections
    _acceptor->listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }

    DoAccept();
}

void Server::DoAccept() {
    // The new connection gets its own strand
    _acceptor->async_accept(net::make_strand(_ioc), beast::bind_front_handler(&Server::OnAccept, shared_from_this()));
}

void Server::OnAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        fail(ec, "accept");
    } else {
        // Create the session and run it
        std::make_shared<Session>(std::move(socket))->Run();
    }

    // Accept another connection
    DoAccept();
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

        boost::asio::io_context ioc;
        _endPoint = tcp::endpoint{address, port};
        _acceptor = std::make_shared<tcp::acceptor>(_ioc);

        StartServerListener();

        // Create and launch a listening port
        //std::make_shared<Listener>(*_ioc, tcp::endpoint{address, port})->Run();

        _ioc.run();
    }
    catch (const std::exception& e)
    {
        errorMessage = "Exception. Location: Server.cpp - Start(). ";
        std::cout << errorMessage << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return status;
}