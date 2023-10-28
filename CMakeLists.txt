cmake_minimum_required(VERSION 3.26)
project(HackerChat)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(PUGIXML_CPP "/opt/devtools/pugixml-1.14/src/pugixml.cpp")

include_directories(/opt/devtools/boost_1.83.0/include
                    /opt/devtools/pugixml-1.14/src)

add_executable(HackerChatServer main.cpp
        Server/Server.cpp
        Server/Server.hpp
        ${PUGIXML_CPP}
        Server/Session.cpp
        Server/Session.hpp)

add_executable(HackerChatClient Client/ClientMain.cpp
        Client/Client.hpp
        Client/Client.cpp
        ${PUGIXML_CPP})
