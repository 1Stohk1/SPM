cmake_minimum_required(VERSION 3.21)
project(fastflow_src)

set(CMAKE_CXX_FLAGS "-O3 -pthread")

set(CMAKE_CXX_STANDARD 20)

add_executable(fastflow main.cpp)
