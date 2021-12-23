cmake_minimum_required(VERSION 3.18)

# panini requires at least C++17

project(Panini)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# check if we're building the library or including it

set(PANINI_BUILDING OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(PANINI_BUILDING ON)
endif()

# allow cmake 3.13+ to override options for FetchContent and add_subdirectories

if(POLICY CMP0077)
	cmake_policy(SET CMP0077 NEW)
endif()

# options

set(
	PANINI_INCLUDE_BUILD_DIR
	"${PROJECT_SOURCE_DIR}/include"
)
set(
	PANINI_INCLUDE_INSTALL_DIR
	"${CMAKE_INSTALL_INCLUDEDIR}"
)

option(
	PANINI_BUILD_EXAMPLES
	"Build the tests in source/examples"
	${PANINI_BUILDING}
)
option(
	PANINI_BUILD_TESTS
	"Build the tests in source/tests"
	${PANINI_BUILDING}
)
option(
	PANINI_INSTALL
	"Install CMake targets during the install step"
	${PANINI_BUILDING}
)

# target

add_library(Panini INTERFACE)
target_include_directories(
	Panini
	INTERFACE include/)

# tests

if(PANINI_BUILD_TESTS)
	# google test dependency

	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
	)
	# prevent overriding the parent project's compiler and linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()
	add_subdirectory(source/tests)
endif()

# examples

if(PANINI_BUILD_EXAMPLES)
	add_subdirectory(source/examples)
endif()