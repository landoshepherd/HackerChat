#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "pugixml.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Client{
public:
    Client();
    ~Client();
    bool Initialize();
    bool Start();

private:
    std::shared_ptr<boost::asio::io_context> m_ioContext;
    std::shared_ptr<tcp::resolver> m_Resolver;
    tcp::resolver::results_type m_EndPoints;
    std::shared_ptr<tcp::socket> m_Socket;
    std::shared_ptr<websocket::stream<tcp::socket>> m_WebSocketStream;
    std::vector<char> m_buffer;
    boost::system::error_code m_error;
    std::string m_destinationHost;
    std::string m_destinationPort;

private:
    bool ConnectToServer();
};