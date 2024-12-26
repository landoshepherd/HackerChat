#include "HCController/HackerChatController.hpp"
#include "HCModel/HackerChatModel.hpp"
#include "WebSocket/WebSocketClient.hpp"

int main(int argc, char* argv[]){
    // Create io context
    net::io_context ioc;

    // Create and configure websocket
    std::string host("127.0.0.1");
    std::string port("8001");
    std::string deviceId("21972c83-d79d-4070-993e-8c429d0b606a");
    std::shared_ptr<WebSocketClient> webSocketClient = std::make_shared<WebSocketClient>(ioc, host, port);

    // Model will be shared by both the view and controller
    HackerChatModel model;

    // Register incoming message callback with websocket
    std::function<void(HCCommonBaseCommand&)> callback = [&model](HCCommonBaseCommand& message){
        model.StoreMessage(message);
    };
    webSocketClient->RegisterIncomingMessageCallback(callback);
    HackerChatController controller(webSocketClient, model, deviceId);
    controller.Start(ioc);

    // Create the view here
    return 0;
}
