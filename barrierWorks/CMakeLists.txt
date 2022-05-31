cmake_minimum_required(VERSION 3.21)
project(barrierWorks)

set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "/usr/lib/gcc/x86_64-pc-cygwin/11/include/c++;/usr/lib/gcc/x86_64-pc-cygwin/11/include/c++/x86_64-pc-cygwin;/usr/lib/gcc/x86_64-pc-cygwin/11/include/c++/backward;/usr/lib/gcc/x86_64-pc-cygwin/11/include;/usr/include;/usr/include/w32api")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "stdc++;gcc_s;gcc;cygwin;advapi32;shell32;user32;kernel32;gcc_s;gcc")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "/usr/lib/gcc/x86_64-pc-cygwin/11;/usr/x86_64-pc-cygwin/lib;/usr/lib;/lib")
set(CMAKE_CXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")

set(CMAKE_CXX_STANDARD 20)

add_executable(barrierWorks main.cpp utimer.cpp utimer.h)
