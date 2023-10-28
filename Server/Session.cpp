//
// Created by Lando Shepherd on 10/25/23.
//
#include "Session.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace http = beast::http;

void Session::AddSocketToSession(tcp::socket& socket){
    m_connections.push_back(std::move(socket));
}

void Session::do_session()
{
    try
    {
        websocket::stream<tcp::socket> ws1{std::move(m_connections[0])};
        websocket::stream<tcp::socket> ws2{std::move(m_connections[1])};

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

        ws1.accept();
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

int Session::GetNumOfConnections(){
    return m_connections.size();
}