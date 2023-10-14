//
// Created by Lando Shepherd on 10/14/23.
//
#include "Client.hpp"

Client::Client()
        : m_buffer(),
          m_error(),
          m_destinationHost(),
          m_destinationPort(){}

Client::~Client() = default;

bool Client::Initialize() {
    bool status = true;
    std::string errorMessage;

    try {
        m_ioContext = std::make_shared<boost::asio::io_context>();
        m_Resolver = std::make_shared<tcp::resolver>(*m_ioContext);
        m_WebSocketStream = std::make_shared<websocket::stream<tcp::socket>>(*m_ioContext);
    }
    catch(std::exception& ex){
        status = false;
        errorMessage = "Exception. Location: Client.cpp - Initialize().\n";
        errorMessage += ex.what();
        std::cout << errorMessage << std::endl;
    }
    return status;
}

bool Client::Start() {
    try
    {
        std::cout << "Starting client..." << std::endl;
        m_destinationHost = "127.0.0.1";
        m_destinationPort = "8001";

        if (ConnectToServer()){
            while(true){
                std::string message;
                std::getline(std::cin, message);

                // Send the message
                m_WebSocketStream->write(net::buffer(message));
            }

            // This buffer will hold the incoming message
            beast::flat_buffer buffer;

            // Read a message into our buffer
            m_WebSocketStream->read(buffer);

            // Close the WebSocket connection
            m_WebSocketStream->close(websocket::close_code::normal);

            // If we get here then the connection is closed gracefully

            // The make_printable() function helps print a ConstBufferSequence
            std::cout << beast::make_printable(buffer.data()) << std::endl;
        }
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool Client::ConnectToServer() {
    bool rc = true;
    boost::system::error_code errorCode;

    try{
        std::cout << "Connecting to server..." << std::endl;
        // Look up the domain name
        auto const results = m_Resolver->resolve(m_destinationHost, m_destinationPort);

        // Make the connection on the IP address we get from a lookup
        auto endpoint = net::connect(m_WebSocketStream->next_layer(), results);
        m_destinationHost += ':' + std::to_string(endpoint.port());

        // Set a decorator to change the User-Agent of the handshake
        m_WebSocketStream->set_option(websocket::stream_base::decorator(
                [](websocket::request_type& req)
                {
                    req.set(http::field::user_agent,
                            std::string(BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
                }));

        // Perform the websocket handshake
        m_WebSocketStream->handshake(m_destinationHost, "/");

        std::cout << "Successfully connected to server." << std::endl;
    }
    catch(std::exception& ex){
        rc = false;
    }

    return rc;
}
