cmake_minimum_required(VERSION 2.8)

include(${CMAKE_SOURCE_DIR}/../buildconfig/detectOS.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../products/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../products/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../products/bin)

include_directories(trace)
include_directories(../3rdparty/jsoncpp-src-0.5.0/include)

add_subdirectory(../3rdparty/jsoncpp-src-0.5.0 ./libjsoncpp)
add_subdirectory(trace)
add_subdirectory(firebaseHelloWorld/backend)