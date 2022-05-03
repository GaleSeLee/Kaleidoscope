cmake_minimum_required(VERSION 3.0)
project(Kaleidoscope)

set(SOURCES 
    src/lexer.cpp
    src/parser.cpp
    src/log.cpp
    src/codegen.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")

include_directories(./include)
include_directories(~/BASE/clangllvm/include)
link_directories(~/BASE/clangllvm/lib)
add_executable(run ${SOURCES})

file(GLOB llvmlib ~/BASE/clangllvm/lib/lib*)
target_link_libraries(run ${llvmlib})

