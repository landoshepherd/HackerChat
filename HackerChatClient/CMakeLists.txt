cmake_minimum_required(VERSION 3.26)
project(HackerChatClient)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

# Check if the external files directory is provided
if(NOT EXTERNAL_FILES_DIR)
    message(FATAL_ERROR "Please provide the path to the external files directory using the EXTERNAL_FILES_DIR environment variable or by passing it as a command-line argument.")
endif()

file(GLOB_RECURSE PUGIXML_CPP "${EXTERNAL_FILES_DIR}/*/pugixml.cpp")
if (NOT PUGIXML_CPP)
    message(FATAL_ERROR "pugixml.cpp not found. Terminating.")
    return()
endif()

file(GLOB_RECURSE RAPID_JSON_H "${EXTERNAL_FILES_DIR}/*/rapidjson.h")
if (NOT RAPID_JSON_H)
    message(FATAL_ERROR "rapidjson.h not found. Terminating.")
    return()
endif()

include_directories(/opt/devtools/boost_1.83.0/include
        /opt/devtools/pugixml-1.14/src
        /opt/devtools
        /opt/devtools/spdlog)

# Find ExampleLib
find_package(Boost 1.83.0 COMPONENTS log)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found. Terminating.")
endif()

# Define your CMake variables
file(GLOB_RECURSE CONFIG_PATH "${CMAKE_HOME_DIRECTORY}/configs/HCClientConfig.json")
if(NOT CONFIG_PATH)
    message(FATAL_ERROR "WebSocket config file not found. Terminating.")
    return()
endif()

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




