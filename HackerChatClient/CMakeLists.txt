cmake_minimum_required(VERSION 3.26)
project(HackerChatClient)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(PUGIXML_CPP "/opt/devtools/pugixml-1.14/src/pugixml.cpp")
set(RAPID_JSON_H "/opt/devtools/rapidjson/rapidjson.h")

include_directories(/opt/devtools/boost_1.83.0/include
        /opt/devtools/pugixml-1.14/src
        /opt/devtools
        /opt/devtools/spdlog)

# Find ExampleLib
find_package(Boost 1.83.0 COMPONENTS log)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(HackerChatClient
            HackerChatClient_main.cpp
            HackerChatClient.hpp
            HackerChatClient.cpp
            ${RAPID_JSON_H}
            ${PUGIXML_CPP}
            WebSocket/WebSocketClient.cpp
            WebSocket/WebSocketClient.hpp)
    target_link_libraries(HackerChatClient ${Boost_LIBRARIES})
endif()



