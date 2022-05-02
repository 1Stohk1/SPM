# set the project name and version
project(Map)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_minimum_required(VERSION 3.12)

configure_file(MapConfig.h.in MapConfig.h)

option(USE_DELAYER "Use the translator with delays of 1sec" ON)

if(USE_DELAYER)
	add_library(Translator translator.1.cpp)
else(USE_DELAYER)
	add_library(Translator translator.0.cpp)
endif()
list(APPEND EXTRA_LIBS Translator)

# add the Timer library
add_subdirectory(lib)

add_executable(Map
		tb_par.cpp)

target_link_libraries(Map PUBLIC Timer
		${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Map PUBLIC
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/lib"
		)
