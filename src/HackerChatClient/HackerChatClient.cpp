
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
#include "HackerChatClient.hpp"
#include "rapidjson.h"
#include "document.h"
#include "HCCommonBaseCommand/HCCommonBaseCommand.h"
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

HackerChatClient::HackerChatClient():
        rootDir(),
        host("127.0.0.1"),
        port("8001"),
        deviceId("21972c83-d79d-4070-993e-8c429d0b606a"),
        stop(false),
        newMessages(){
}

bool HackerChatClient::Load(const std::string& configFilename){
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
            host = doc["host"].GetString();
            port = doc["port"].GetString();
            deviceId = doc["deviceId"].GetString();

            BOOST_LOG_TRIVIAL(trace) << "Successfully loading chat client configuration.";
        }
        else{
            rc = false;
            BOOST_LOG_TRIVIAL(error) << "Failed to load HackerChatClient config file.";
        }
    }
    else{
        rc = false;
        BOOST_LOG_TRIVIAL(error) << "HackerChatClient config file not found.";
    }

    return rc;
}

int HackerChatClient::Start() {
    try {
        using FuncPtr = void(*)();
        auto const text = "Hello!";
        stop = false; //Will need thread protection

        net::io_context ioc;
        webSocketClient = std::make_shared<WebSocketClient>(ioc, host, port, text);
        webSocketClient->Start();

        std::thread userThread([this]() {
            Proc();
        });

        std::thread messageProcessingThread([this](){
           ProcessMessageQueue();
        });

        ioc.run();
        userThread.join();
        //webSocketThread.join();
    }
    catch(std::exception& ex){
        std::cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void HackerChatClient::ProcessMessageQueue(){
    while(true){
        webSocketClient->GetQueueLock().lock();
        std::queue<HCCommonBaseCommand>& wsQueue = webSocketClient->GetMessageQueue();
        while(!wsQueue.empty()){
            newMessages.push(wsQueue.front());
            wsQueue.pop();
        }
        webSocketClient->GetQueueLock().unlock();
        //Sleep to allow the possibility of the other thread obtaining the lock
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void HackerChatClient::Proc(){
    //Wait here until the websocket client makes a connection to the server
    webSocketClient->GetWebSocketProcLock().lock();
    std::string message;
    while(!stop){
        while(!newMessages.empty()){
            std::cout << newMessages.front()._getMessage() << std::endl;
            newMessages.pop();
        }

        std::cout << "Enter message here: " << std::endl;
        std::getline(std::cin, message);
        // Create a message command
        boost::uuids::random_generator gen;
        boost::uuids::uuid source = gen();
        boost::uuids::uuid destination = gen();
        HCCommonBaseCommand command(source, destination, message);
        std::string commandAsString = HCCommonBaseCommand::_serialize(command);
        this->webSocketClient->SendMessage(commandAsString);
    }
}


