
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
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
    _rootDir(),
    _host(),
    _port(),
    _deviceId(),
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
        _InitializeLogging();

        char* rootDir = std::getenv("HACKERCHAT_CLIENT_ROOT");
        _rootDir = rootDir;
        if(_rootDir.empty()){
            BOOST_LOG_TRIVIAL(error) << "HACKERCHAT_CLIENT_ROOT environment variable not defined. Terminating program.";
        }

        std::string configDirPath = _rootDir.append("/configs/HCClientConfig.json");
        /*std::string configPathString(configDir);
        configPathString.append("/HCClientConfig.json");*/

        if (_Load(configDirPath)) {
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

void HackerChatClient::_InitializeLogging() {
    // Set up logging to file
    boost::log::add_file_log(
            boost::log::keywords::file_name = "sample.log",
            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
    );

    boost::log::core::get()->set_filter(
            boost::log::trivial::severity >= boost::log::trivial::info
    );
}


