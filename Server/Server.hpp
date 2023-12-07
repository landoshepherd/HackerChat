#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <vector>

#include "pugixml.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Server : public std::enable_shared_from_this<Server>{
public:
    Server();
    ~Server();
    bool InitializeServer();
    void do_session(std::vector<tcp::socket>& connections);
    void StartServerListener();
    void fail(beast::error_code ec, char const* what);
    void DoAccept();
    void OnAccept(beast::error_code ec, tcp::socket socket);
    bool Start();

private:
    boost::asio::io_context _ioc;
    std::shared_ptr<tcp::acceptor> _acceptor;
    std::shared_ptr<tcp::socket> m_socket;
    std::shared_ptr<websocket::stream<tcp::socket>> m_webSocketStream;
    tcp::endpoint _endPoint;
    boost::system::error_code m_errorCode;
    int m_numOfActiveSessions;
};