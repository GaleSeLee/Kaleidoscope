cmake_minimum_required(VERSION 3.0)
project(Kaleidoscope)

set(SOURCES 
    src/lexer.cpp
    src/parser.cpp
    src/log.cpp
)

include_directories(./include)

add_executable(parser ${SOURCES})
