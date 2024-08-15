cmake_minimum_required(VERSION 3.27)
project(HackerChatCommon)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE RAPID_JSON_H "${EXTERNAL_FILES_DIR}/*/rapidjson.h")
if (NOT RAPID_JSON_H)
    message(FATAL_ERROR "rapidjson.h not found. Terminating.")
    return()
endif()

include_directories(${EXTERNAL_FILES_DIR}/boost_1.83.0/include
                    ${EXTERNAL_FILES_DIR}/rapidjson)

add_library(HackerChatCommon STATIC library.cpp
        HCCommonBaseCommand/HCCommonBaseCommand.cpp
        HCCommonBaseCommand/HCCommonBaseCommand.h
)

add_executable(HackerChatTestApp
                HCCommonBaseCommand/HCCommonBaseCommand.cpp
                main.cpp)