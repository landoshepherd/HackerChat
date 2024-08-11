//
// Created by Lando Shepherd on 11/10/23.
//

#include "Listener.hpp"


void fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

Listener::Listener(net::io_context &ioc, tcp::endpoint endpoint)
        : ioc_(ioc), acceptor_(ioc) {

    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        fail(ec, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind");
        return;
    }

    // _Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen");
        return;
    }
}

// _Start accepting incoming connections
void Listener::Run() {
    DoAccept();
}

void Listener::DoAccept() {
    // The new connection gets its own strand
    acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&Listener::OnAccept, shared_from_this()));
}

void Listener::OnAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
        fail(ec, "accept");
    } else {
        // Create the session and run it
        activeConnections_.insert(std::make_pair<std::string, std::shared_ptr<Connection>>("sample", std::make_shared<Connection>(std::move(socket))));
        activeConnections_.at("sample")->Run();
    }

    // Accept another connection
    DoAccept();
}