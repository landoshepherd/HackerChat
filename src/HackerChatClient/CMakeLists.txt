cmake_minimum_required(VERSION 3.26)
project(HackerChatClient)
set(HACKER_CHAT_MAIN_DIR ..)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

# Check if the external files directory is provided
if(NOT EXTERNAL_LIBRARIES)
    message(FATAL_ERROR "Please provide the path to the external libraries directory using the
    EXTERNAL_LIBRARIES environment variable or by passing it as a command-line argument.")
endif()

# Find rapid json header files
file(GLOB_RECURSE RAPID_JSON_H "${EXTERNAL_LIBRARIES}/*/rapidjson.h")
if (NOT RAPID_JSON_H)
    message(FATAL_ERROR "rapidjson.h not found. Terminating.")
    return()
endif()

# Find boost log package
set(CMAKE_PREFIX_PATH ${EXTERNAL_LIBRARIES})
find_package(Boost 1.83.0 COMPONENTS log)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found. Terminating.")
endif()

# Include boost header files
include_directories(${Boost_INCLUDE_DIRS}
                    ${EXTERNAL_LIBRARIES}/rapidjson
                    ../HackerChatCommon/HCCommonBaseCommand/include)

# Generate the HackerChatClient executable
add_executable(HackerChatClient
        HackerChatClient_main.cpp
        HackerChatClient.cpp
        WebSocket/WebSocketClient.cpp)

# Link boost libraries with executables
target_link_libraries(HackerChatClient ${Boost_LIBRARIES}
                    ${HACKERCHAT_MAIN_DIR}/HackerChatCommon/HCCommonBaseCommand/lib/libHackerChatCommon.a)




