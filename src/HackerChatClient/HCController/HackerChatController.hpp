#include <boost/beast/core.hpp>
//#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "WebSocketClient.hpp"
#include "HCModel/HackerChatModel.hpp"
#include "HCView/HackerChatMainView.hpp"
#include "HCCommonBaseCommand.hpp"
#include "MessageQueue.hpp"

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
    std::shared_ptr<WebSocketClient> m_webSocketClient;
    std::string m_rootDir;
    std::string m_deviceId;
    bool m_stop;
    HackerChatModel m_model;
    HackerChatMainView m_mainView;
    std::mutex m_incomingMessagesQueueLock;
    std::shared_ptr<MessageQueue> m_incomingMessages;
    std::shared_ptr<MessageQueue> m_outgoingMessages;
public:
    // Resolver and socket require an io_context
    HackerChatController(std::shared_ptr<WebSocketClient> webSocketClient,
                         HackerChatModel& model,
                         std::string& deviceId,
                         std::shared_ptr<MessageQueue> incomingMessageQueue,
                         std::shared_ptr<MessageQueue> outgoingMessageQueue);
    ~HackerChatController() = default;
    bool Load(const std::string& configFilename);
    int Start(net::io_context& ioc);
    void Proc();
    void InitializeLogging();
    void SendMessage(const HCCommonBaseCommand& command) const;
};
