cmake_minimum_required(VERSION 3.26)
project(HackerChat)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(PUGIXML_CPP "/opt/devtools/pugixml-1.14/src/pugixml.cpp")
set(RAPID_JSON_H "/opt/devtools/rapidjson/rapidjson.h")

include_directories(/opt/devtools/boost_1.83.0/include
                    /opt/devtools/pugixml-1.14/src)

add_executable(HackerChatServer main.cpp
        HackerChatServer/Server.cpp
        HackerChatServer/Server.hpp
        ${RAPID_JSON_H}
        ${PUGIXML_CPP}
        HackerChatServer/Session.cpp
        HackerChatServer/Session.hpp
        HackerChatServer/Listener.cpp
        HackerChatServer/Listener.hpp)

add_executable(HackerChatClient HackerChatClient/ClientMain.cpp
        HackerChatClient/HackerChatClient.hpp
        HackerChatClient/HackerChatClient.cpp
        ${RAPID_JSON_H}
        ${PUGIXML_CPP}
        WebSocket/WebSocketClient.cpp
        WebSocket/WebSocketClient.hpp)

add_executable(TestServer test_server.cpp)
