cmake_minimum_required(VERSION 3.21)
project(Assignment6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -fopenmp")

add_executable(Assignment6 implementation.cpp)
