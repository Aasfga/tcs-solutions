cmake_minimum_required(VERSION 3.15)
project(decomposition)

set(CMAKE_CXX_STANDARD 11)
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(CGAL REQUIRED COMPONENTS Core)

add_executable(decomposition main.cpp)
target_link_libraries(decomposition CGAL::CGAL)