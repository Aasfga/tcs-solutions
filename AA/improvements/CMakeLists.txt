cmake_minimum_required(VERSION 3.15)
project(improvements)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(CGAL REQUIRED COMPONENTS Core)

add_executable(improvements main.cpp)
target_link_libraries(improvements CGAL::CGAL)