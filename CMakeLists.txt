cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

project (chrysanth)

if (MSVC)
	add_definitions(" /arch:AVX2 /std:c++14")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_definitions(" -std=c++14")
endif()

include_directories("${PROJECT_SOURCE_DIR}")
include_directories(external)

add_subdirectory(test)
