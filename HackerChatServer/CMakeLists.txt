cmake_minimum_required(VERSION 3.26)
project(HackerChatServer)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(PUGIXML_CPP "/opt/devtools/pugixml-1.14/src/pugixml.cpp")
set(RAPID_JSON_H "/opt/devtools/rapidjson/rapidjson.h")

include_directories(/opt/devtools/boost_1.83.0/include
                    /opt/devtools/pugixml-1.14/src)

find_package(Boost REQUIRED COMPONENTS log)

add_executable(HackerChatServer main.cpp
        Server.cpp
        Server.hpp
        ${RAPID_JSON_H}
        ${PUGIXML_CPP}
        Session.cpp
        Session.hpp
        Listener.cpp
        Listener.hpp)

