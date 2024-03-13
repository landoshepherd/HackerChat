#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include "/opt/devtools/rapidjson/rapidjson.h"
#include "/opt/devtools/rapidjson/document.h"
#include <string>

#include "WebSocket/WebSocketClient.hpp"

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
    std::string _host;
    std::string _port;
    std::string _deviceId;
    bool _stop;
public:
    // Resolver and socket require an io_context
    HackerChatClient();
    ~HackerChatClient() = default;
    bool _Load(const std::string& configFilename);
    int _Start();
    void _Proc();
    //bool _SendMessage(std::string& message, std::string& statusMessage);
};