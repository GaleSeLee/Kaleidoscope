cmake_minimum_required(VERSION 3.0)
project(Kaleidoscope)

set(SOURCES 
    src/lexer.cpp
    src/parser.cpp
    src/log.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")

include_directories(./include)

add_executable(parser ${SOURCES})
