cmake_minimum_required(VERSION 2.8)
project(FirebaseHelloWorld)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(HEADERS
	IRequest.h
	Request.h
	RequestFactory.h
	Exceptions.h
	FirebaseClient.h
)
set(SOURCES
	Request.cpp
	RequestFactory.cpp
	FirebaseClient.cpp
	main.cpp
)
set(LIBRARIES
	curl
	jsoncpp
	Trace
)

add_executable(firebaseHelloWorld ${HEADERS} ${SOURCES})

target_link_libraries(firebaseHelloWorld ${LIBRARIES})