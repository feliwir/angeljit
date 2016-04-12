cmake_minimum_required(VERSION 3.1)

project(angeljit)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

add_subdirectory(src)

option(BUILD_TEST "Enable to build test" OFF)

if(BUILD_TEST)
	add_subdirectory(test)
endif()