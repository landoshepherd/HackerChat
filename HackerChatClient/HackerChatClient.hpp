#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "../WebSocket/WebSocketClient.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//-----------------------------------------------------------------------------
// Sends a WebSocket message and prints the response
class HackerChatClient
{
private:
    std::shared_ptr<WebSocketClient> _webSocketClient;
    bool _stop;
public:
    // Resolver and socket require an io_context
    HackerChatClient();
    ~HackerChatClient() = default;
    int Start();
    //bool _SendMessage(std::string& message, std::string& statusMessage);
};