#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "HackerChatClient.hpp"
#include "../WebSocket/WebSocketClient.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

int HackerChatClient::Start() {
    //TODO: Eventually, we will load a websocket config file here to configure the websocket client
    auto const host = "127.0.01";
    auto const port = "8001";
    auto const text = "What up!";

    net::io_context ioc;
    std::make_shared<WebSocketClient>(ioc, host, port, text)->run();

    ioc.run();

    return EXIT_SUCCESS;
}
