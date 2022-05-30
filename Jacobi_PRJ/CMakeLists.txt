cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

set(CXX = g++)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "-std=c++20 -pthread ${CMAKE_CXX_FLAGS}")

include(CTest)
enable_testing()

add_executable(project sequential.cpp)

add_executable(project parallel_std.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
