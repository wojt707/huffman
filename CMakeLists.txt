cmake_minimum_required(VERSION 3.0.0)
project(Huffman VERSION 0.1.0)

include(CTest)
enable_testing()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY})

add_executable(Huffman main.cpp library.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
