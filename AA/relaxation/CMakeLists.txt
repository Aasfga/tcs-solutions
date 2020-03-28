cmake_minimum_required(VERSION 3.15)
project(relaxation)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(CGAL REQUIRED COMPONENTS Core)

add_executable(relaxation main.cpp)
target_link_libraries(relaxation CGAL::CGAL)