cmake_minimum_required(VERSION 3.7)
project(J)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/galaxy.cpp src/output.cpp)
add_executable(J ${SOURCE_FILES})