cmake_minimum_required(VERSION 3.27)
project(HackerChatCommon)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

# Check if the external files directory is provided
if(NOT EXTERNAL_LIBRARIES)
    message(FATAL_ERROR "Please provide the path to the external files directory using the
    EXTERNAL_DEPENDENCIES environment variable or by passing it as a command-line argument.")
endif()

if(NOT HACKERCHAT_LIBRARIES)
    message(FATAL_ERROR "Provide the path to the HackerChat libraries")
endif()

file(GLOB_RECURSE RAPID_JSON_H "${EXTERNAL_LIBRARIES}/*/rapidjson.h")
if (NOT RAPID_JSON_H)
    message(FATAL_ERROR "rapidjson.h not found. Terminating.")
    return()
endif()

include_directories(${EXTERNAL_LIBRARIES}/boost_1.83.0/include
                    ${EXTERNAL_LIBRARIES}/rapidjson)

set(CMAKE_PREFIX_PATH /opt/libraries)
find_package(Boost 1.83.0 COMPONENTS log serialization)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost libraries not found. Terminating.")
endif()

add_library(HackerChatCommon STATIC
        HCCommonBaseCommand/src/HCCommonBaseCommand.cpp
        HCCommonBaseCommand/include/HCCommonBaseCommand.h
)

target_link_libraries(HackerChatCommon ${Boost_LIBRARIES})

# Move compiled library to lib directory
if (EXISTS "${CMAKE_SOURCE_DIR}/build/libHackerChatCommon.a")
    file(RENAME ${CMAKE_SOURCE_DIR}/build/libHackerChatCommon.a ${CMAKE_SOURCE_DIR}/HCCommonBaseCommand/lib/libHackerChatCommon.a)
    file(REMOVE ${CMAKE_SOURCE_DIR}/build/libHackerChatCommon.a)
else()
    message("Failed to build HackerChatCommon library.")
endif()

message("TEST: ${HACKERCHAT_LIBRARIES}")
# Move header files and library to central library repo
file(COPY ${CMAKE_SOURCE_DIR}/HCCommonBaseCommand/lib/libHackerChatCommon.a
        DESTINATION ${HACKERCHAT_LIBRARIES}/HCCommonBaseCommand/lib)
file(COPY ${CMAKE_SOURCE_DIR}/HCCommonBaseCommand/include
        DESTINATION ${HACKERCHAT_LIBRARIES}/HCCommonBaseCommand)