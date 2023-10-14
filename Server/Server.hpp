#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Server{
public:
    Server();
    ~Server();
    bool InitializeServer();
    void do_session(tcp::socket& socket);
    bool Start();

private:
    std::shared_ptr<boost::asio::io_context> m_context;
    std::shared_ptr<tcp::acceptor> m_acceptor;
    std::shared_ptr<tcp::socket> m_socket;
    std::shared_ptr<websocket::stream<tcp::socket>> m_webSocketStream;
    boost::system::error_code m_errorCode;
};