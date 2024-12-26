
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
#include "HCCommonBaseCommand/HCCommonBaseCommand.h"
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
                                           std::string& deviceId):
        rootDir(),
        deviceId(deviceId),
        stop(false),
        newMessages(),
        model(model),
        webSocketClient(std::move(webSocketClient)){
}

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
            deviceId = doc["deviceId"].GetString();

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
        using FuncPtr = void(*)();
        auto const text = "Hello!";
        stop = false; //Will need thread protection

        //Register callback for incoming messages
        webSocketClient->Start();

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
    while(!webSocketClient->Connected()){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::string message;
    while(!stop){
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


