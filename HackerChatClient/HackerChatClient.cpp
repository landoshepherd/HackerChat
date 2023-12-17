#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "HackerChatClient.hpp"
#include "../WebSocket/WebSocketClient.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

HackerChatClient::HackerChatClient():
    _stop(false){
}

int HackerChatClient::Start() {
    try {
        //TODO: Eventually, we will load a websocket config file here to configure the websocket client
        auto const host = "127.0.0.1";
        auto const port = "8001";
        auto const text = "Hello!";
        _stop = false; //Will need thread protection

        net::io_context ioc;
        _webSocketClient = std::make_shared<WebSocketClient>(ioc, host, port, text);
        _webSocketClient->_Start();

        std::thread userThread([this](){
            _Proc();
        });

        ioc.run();
        userThread.join();
    }
    catch(std::exception ex){
        std::cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void HackerChatClient::_Proc(){
    std::string message;
    while(!_stop){
        std::getline(std::cin, message);
        this->_webSocketClient->_SendMessage(message);
    }
}


