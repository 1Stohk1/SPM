cmake_minimum_required(VERSION 3.21)
project(Assignment4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fopenmp -O3")

add_executable(Assignment4_0 thread_pool.cpp)
add_executable(Assignment4_1 thread_pool_omp.cpp)
add_executable(Placeholder placeholder_pool.cpp)
add_executable(Omp omp.cpp)
