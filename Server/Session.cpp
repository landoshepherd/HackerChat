//
// Created by Lando Shepherd on 10/25/23.
//
#include "Session.hpp"
#include <thread>
#include <chrono>

// Report a failure
void Session::fail(beast::error_code ec, char const* what)
{
    std::cout << what << ": " << ec.message() << "\n";
}

// Take ownership of the socket
Session::Session(tcp::socket&& socket)
:ws_(std::move(socket)){}

// Get on the correct executor
void Session::Run() {
    // We need to be executing within a strand to perform async operations
    // on the I/O objects in this session. Although not strictly necessary
    // for single-threaded contexts, this example code is written to be
    // thread-safe by default.
    net::dispatch(ws_.get_executor(), beast::bind_front_handler(&Session::OnRun, shared_from_this()));
}

// Start the asynchronous operation
void Session::OnRun() {
    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(websocket::stream_base::decorator(
            [](websocket::response_type &res) {
                res.set(http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-server-async");
            }));
    // Accept the websocket handshake
    ws_.async_accept(beast::bind_front_handler(&Session::OnAccept, shared_from_this()));
}

void Session::OnAccept(beast::error_code ec) {
    if (ec) {
        return fail(ec, "accept");
    }
    // Read a message
    DoRead();
}

void Session::DoRead() {
    // Read a message into our buffer
    ws_.async_read(buffer_, beast::bind_front_handler(&Session::OnRead, shared_from_this()));
}

void Session::OnRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if (ec == websocket::error::closed) {
        return;
    }

    if (ec) {
        return fail(ec, "read");
    }

    // Echo the message
    std::cout << beast::buffers_to_string(buffer_.data()) << std::endl;
    ws_.text(ws_.got_text());
    ws_.async_write(buffer_.data(), beast::bind_front_handler(&Session::OnWrite, shared_from_this()));
}

void Session::OnWrite(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) {
        return fail(ec, "write");
    }

    // Clear the buffer
    buffer_.consume(buffer_.size());

    // Do another read
    DoRead();
}