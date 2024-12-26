#include <boost/beast/core.hpp>
//#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "WebSocket/WebSocketClient.hpp"
#include "HCModel/HackerChatModel.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//-----------------------------------------------------------------------------
// Sends a WebSocket message and prints the response
class HackerChatController
{
private:
    std::shared_ptr<WebSocketClient> webSocketClient;
    std::string rootDir;
    std::string deviceId;
    bool stop;
    std::queue<HCCommonBaseCommand> newMessages;
    HackerChatModel model;
public:
    // Resolver and socket require an io_context
    HackerChatController(std::shared_ptr<WebSocketClient> webSocketClient,
                         HackerChatModel& model,
                         std::string& deviceId);
    ~HackerChatController() = default;
    bool Load(const std::string& configFilename);
    int Start(net::io_context& ioc);
    void Proc();
    void InitializeLogging();
    //bool _SendMessage(std::string& message, std::string& statusMessage);
};
