#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

class Client{
public:
    Client();
    ~Client();
    bool InitializeClient(std::string& serverName, std::string& serviceName);
    bool Start();

private:
    std::shared_ptr<boost::asio::io_context> m_context;
    std::shared_ptr<tcp::resolver> m_tcpResolver;
    tcp::resolver::results_type m_endPoints;
    std::shared_ptr<tcp::socket> m_socket;
    std::vector<char> m_buffer;
    bool started;
    boost::system::error_code m_error;

private:
    bool ResolveEndPoints(std::string& serviceName);
    bool ConnectToSocket();
};