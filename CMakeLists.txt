cmake_minimum_required(VERSION 3.0)
project(Kaleidoscope)

set(SOURCES 
    src/lexer.cpp
    src/parser.cpp
    src/log.cpp
    src/codegen.cpp)

find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")

include_directories(./include)
add_executable(run ${SOURCES})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(run ${llvm_libs})
