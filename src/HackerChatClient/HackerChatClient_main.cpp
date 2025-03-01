#include "HCController/HackerChatController.hpp"
#include "HCModel/HackerChatModel.hpp"
#include "WebSocketClient.hpp"
#include "MessageQueue.hpp"

int main(int argc, char* argv[]){
    // Create io context
    net::io_context ioc;

    // Create message queues
    std::shared_ptr<MessageQueue> incomingMessageQueue = std::make_shared<MessageQueue>();
    std::shared_ptr<MessageQueue> outgoingMessageQueue = std::make_shared<MessageQueue>();

    // Create and configure websocket
    std::string host("127.0.0.1");
    std::string port("8001");
    std::string deviceId("21972c83-d79d-4070-993e-8c429d0b606a");
    std::shared_ptr<WebSocketClient> webSocketClient = std::make_shared<WebSocketClient>(ioc, host, port, incomingMessageQueue, outgoingMessageQueue);

    // Model will be shared by both the view and controller
    HackerChatModel model;


    HackerChatController controller(webSocketClient, model, deviceId, incomingMessageQueue, outgoingMessageQueue);
    controller.Start(ioc);

    // Create the view here
    return 0;
}
