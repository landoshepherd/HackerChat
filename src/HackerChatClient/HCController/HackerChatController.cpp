
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "HackerChatController.hpp"
#include "rapidjson.h"
#include "document.h"
#include "HCCommonBaseCommand.hpp"
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

HackerChatController::HackerChatController(std::shared_ptr<WebSocketClient> webSocketClient,
                                           HackerChatModel& model,
                                           std::string& deviceId,
                                           std::shared_ptr<MessageQueue> pIncomingMessageQueue,
                                           std::shared_ptr<MessageQueue> pOutgoingMessageQueue):
        m_rootDir(),
        m_deviceId(deviceId),
        m_stop(false),
        m_model(model),
        m_webSocketClient(std::move(webSocketClient)),
        m_incomingMessages(pIncomingMessageQueue),
        m_outgoingMessages(pOutgoingMessageQueue){}

bool HackerChatController::Load(const std::string& configFilename){
    bool rc = true;

    BOOST_LOG_TRIVIAL(trace) << "Loading chat client config file...";

    if (std::filesystem::exists(configFilename)){
        std::fstream filestream;
        std::stringstream configBuffer;
        rapidjson::Document doc;

        filestream.open(configFilename);

        if (filestream.is_open()){
            configBuffer << filestream.rdbuf();
            doc.Parse(configBuffer.str().c_str());
//            host = doc["host"].GetString();
//            port = doc["port"].GetString();
            m_deviceId = doc["deviceId"].GetString();

            BOOST_LOG_TRIVIAL(trace) << "Successfully loading chat client configuration.";
        }
        else{
            rc = false;
            BOOST_LOG_TRIVIAL(error) << "Failed to load HackerChatController config file.";
        }
    }
    else{
        rc = false;
        BOOST_LOG_TRIVIAL(error) << "HackerChatController config file not found.";
    }

    return rc;
}

int HackerChatController::Start(net::io_context& ioc) {
    try {
        // Register incoming message callback with websocket
        std::function<void(HCCommonBaseCommand&)> incomingMessageCallback = [this](HCCommonBaseCommand& message){
            // There needs to be a lock for this
            m_incomingMessagesQueueLock.lock();
            if(incomingMessagesQueue.size() > 5){
                incomingMessagesQueue.pop();
            }
            incomingMessagesQueue.push(message);
            m_incomingMessagesQueueLock.unlock();
        };
        m_webSocketClient->RegisterIncomingMessageCallback(incomingMessageCallback);

        // Register outgoing message callback with websocket
        std::function<void(HCCommonBaseCommand&)> outgoingMessageCallback = [this](HCCommonBaseCommand& message) {
            m_model.StoreMessage(message);
            m_mainView.UpdateMainView();
        };
        m_webSocketClient->RegisterOutgoingMessageCallback(outgoingMessageCallback);

        using FuncPtr = void(*)();
        auto const text = "Hello!";
        m_stop = false; //Will need thread protection

        //Register callback for incoming messages
        m_webSocketClient->Start();

        //Allow for websocket to make a connection;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::thread userThread([this]() {
            Proc();
        });

        ioc.run();
        userThread.join();
    }
    catch(std::exception& ex){
        std::cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void HackerChatController::Proc(){
    //Wait here until the websocket client makes a connection to the server
    uint8_t retryCount = 0;
    while(!m_webSocketClient->Connected() && retryCount < 3){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        retryCount++;
    }
    std::string message;
    while(!m_stop){
        std::cout << "Look at this" << std::endl;
        // Display main view
        m_mainView.DisplayMainView();

        // Get user input
        std::getline(std::cin, message);

        // Create a message command
        boost::uuids::random_generator gen;
        boost::uuids::uuid source = gen();
        boost::uuids::uuid destination = gen();
        HCCommonBaseCommand command(source, destination, message);

        this->m_webSocketClient->SendMessage(command);

        // Store message in message list model
        m_model.StoreMessage(command);

        // Update main view
        m_mainView.UpdateMainView();

        // Clear the getline buffer
        message.clear();
    }
}

void HackerChatController::SendMessage(const HCCommonBaseCommand& command) const {
    m_outgoingMessages->push(command);
}

