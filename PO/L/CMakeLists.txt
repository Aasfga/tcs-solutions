cmake_minimum_required(VERSION 3.7)
project(L)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Rational.hpp)
add_executable(L ${SOURCE_FILES})