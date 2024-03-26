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


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

HackerChatClient::HackerChatClient():
    _host(),
    _port(),
    _deviceId(),
    _stop(false){
}

bool HackerChatClient::_Load(const std::string& configFilename){
    bool rc = true;

    //BOOST_LOG_TRIVIAL(trace) << "Loading chat client config file...\n";
    if (std::filesystem::exists(configFilename)){
        std::fstream filestream;
        std::stringstream configBuffer;
        rapidjson::Document doc;

        filestream.open(configFilename);

        if (filestream.is_open()){
            configBuffer << filestream.rdbuf();
            doc.Parse(configBuffer.str().c_str());
            _host = doc["_host"].GetString();
            _port = doc["_port"].GetString();
            _deviceId = doc["_deviceId"].GetString();

            //spdlog::warn("Successfully loading chat client configuration.");
        }
        else{
            rc = false;
            //BOOST_LOG_TRIVIAL(error) << "Failed to open HackerChatClient config file.";
            std::cout << "Failed to load HackerChatClient config file." << std::endl;
        }
    }
    else{
        rc = false;
        //BOOST_LOG_TRIVIAL(error) << "HackerChatClient config file does not exist.";
        std::cout << "HackerChatClient config file not found." << std::endl;
    }

    return rc;
}

int HackerChatClient::_Start() {
    try {
        // Set up logger
        //boost::log::add_console_log(std::clog, boost::log::keywords::format = "%TimeStamp% [%Severity%]: %Message%");

        std::filesystem::path configPath = std::filesystem::current_path();
        configPath = configPath.parent_path();
        std::string configPathString = configPath.string();
        configPathString.append("/configs/HCClientConfig.json");

        if (_Load(configPathString)) {
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
        this->_webSocketClient->_SendMessage(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


