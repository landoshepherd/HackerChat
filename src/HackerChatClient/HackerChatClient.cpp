
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
#include "HCCommonBaseCommand.h"
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

HackerChatClient::HackerChatClient():
    _rootDir(),
    _host("127.0.0.1"),
    _port("8001"),
    _deviceId("21972c83-d79d-4070-993e-8c429d0b606a"),
    _stop(false){
}

bool HackerChatClient::_Load(const std::string& configFilePath){
    bool rc = true;

    BOOST_LOG_TRIVIAL(trace) << "Loading chat client config file...";

    if (std::filesystem::exists(configFilePath)){
        std::fstream filestream;
        std::stringstream configBuffer;
        rapidjson::Document doc;

        filestream.open(configFilePath);

        if (filestream.is_open()){
            configBuffer << filestream.rdbuf();
            doc.Parse(configBuffer.str().c_str());
            _host = doc["_host"].GetString();
            _port = doc["_port"].GetString();
            _deviceId = doc["_deviceId"].GetString();

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

int HackerChatClient::_Start() {
    try {
        auto const text = "Hello!";
        _stop = false; //Will need thread protection

        net::io_context ioc;
        _webSocketClient = std::make_shared<WebSocketClient>(ioc, _host, _port, text);
        _webSocketClient->_Start();

        std::thread userThread([this]() {
            _Proc();
        });

        ioc.run();
        userThread.join();
    }
    catch(std::exception ex){
        std::cout << ex.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void HackerChatClient::_Proc(){
    std::string message;
    while(!_stop){
        std::getline(std::cin, message);
        // Create a message command
        boost::uuids::random_generator gen;
        boost::uuids::uuid source = gen();
        boost::uuids::uuid destination = gen();
        HCCommonBaseCommand command(source, destination, message);
        std::string commandAsString = HCCommonBaseCommand::_serialize(command);
        this->_webSocketClient->_SendMessage(commandAsString);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


