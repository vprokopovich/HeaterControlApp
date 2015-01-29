cmake_minimum_required(VERSION 2.8)
project(jsoncpp)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(../)
include_directories(include/)

set(HEADERS
)
set(SOURCES
		src/lib_json/json_reader.cpp
        src/lib_json/json_value.cpp
		src/lib_json/json_writer.cpp
)

#SET(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/../products/lib")

add_library(jsoncpp STATIC ${SOURCES})