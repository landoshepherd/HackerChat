//
// Created by Lando Shepherd on 10/25/23.
//
#include "Session.hpp"


Session::Session()
        : m_initialized(false) {}

void Session::Initialize() {
    m_io_context = std::make_shared<boost::asio::io_context>(1);
    m_initialized = true;
}

bool Session::AddSocketToSession(tcp::socket &socket) {
    bool status = true;
    if (m_initialized) {
        Connection node;
        node.connectionSocket.reset(&socket);
        node.connectionStream = std::make_shared<websocket::stream<beast::tcp_stream>>(*m_io_context);
        m_connections.push_back(std::move(node));
    }
    else{
        status = false;
        std::cout << "Error. Failed to add socket to session. Session not initialized." << std::endl;
    }

    return status;
}

void Session::DoSession(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                        std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream){
    while (true) {
        DoRead(readStream, writeStream);

        // This buffer will hold the incoming message
        beast::flat_buffer buffer1;

        // Read a message from client 1
        readStream->read(buffer1);
        std::string incomingMessage = beast::buffers_to_string(buffer1.data());
        std::cout << "Received from client 1: " << incomingMessage << std::endl;

        writeStream->write(buffer1.data());

        buffer1.clear();
    }
}

void Session::DoRun() {
    try {
        if (!m_initialized) {
            std::cout << "Error. Session not initialized." << std::endl;
            return;
        }

        std::shared_ptr<websocket::stream<beast::tcp_stream>> ws1_ptr = m_connections[0].connectionStream;
        std::shared_ptr<websocket::stream<beast::tcp_stream>> ws2_ptr = m_connections[1].connectionStream;
        net::dispatch(ws1_ptr->get_executor(), beast::bind_front_handler(&Session::DoSession, ws1_ptr, ws2_ptr, shared_from_this()));
        net::dispatch(ws2_ptr->get_executor(), beast::bind_front_handler(&Session::DoSession, ws2_ptr, ws1_ptr, shared_from_this()));

        std::cout << "Websocket handshakes accepted." << std::endl;


    }
    catch (beast::system_error const &se) {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int Session::GetNumOfConnections() {
    return m_connections.size();
}

void Session::DoRead(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                     std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream) {
    beast::flat_buffer buffer1;
    readStream->async_read(buffer1, beast::bind_front_handler(&Session::DoWrite, writeStream, readStream, buffer1, shared_from_this()));
}

void Session::DoWrite(std::shared_ptr<websocket::stream<beast::tcp_stream>> readStream,
                     std::shared_ptr<websocket::stream<beast::tcp_stream>> writeStream,
                     beast::flat_buffer buffer1){
    //readStream->text(readStream->got_text());
    writeStream->async_write(buffer1.data(), beast::bind_front_handler(&Session::ClearBuffer, buffer1, shared_from_this()));
}

void Session::ClearBuffer(beast::flat_buffer buffer1){
    // Clear the buffer
    buffer1.consume(buffer1.size());

    // Do another read
    DoRead();
}

void Session::Start(){
    if (m_initialized) {
        // Setup websocket stream 1
        std::shared_ptr<websocket::stream<beast::tcp_stream>> ws1_ptr = m_connections[0].connectionStream;

        // Set suggested timeout settings for the websocket
        ws1_ptr->set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        ws1_ptr->set_option(websocket::stream_base::decorator([](websocket::response_type &res) {
                                                                res.set(http::field::server,
                                                                std::string(BOOST_BEAST_VERSION_STRING) +
                                                                "websocket-server-async");
                                                                }));

        // Setup websocket stream 2
        std::shared_ptr<websocket::stream<beast::tcp_stream>> ws2_ptr = m_connections[1].connectionStream;

        // Set suggested timeout settings for the websocket
        ws2_ptr->set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        ws2_ptr->set_option(websocket::stream_base::decorator([](websocket::response_type &res) {
                                                                res.set(http::field::server,
                                                                std::string(BOOST_BEAST_VERSION_STRING) +
                                                                "websocket-server-async");
                                                                }));

        DoRun();
    }
}