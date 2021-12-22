cmake_minimum_required(VERSION 3.18)

set(PANINI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${PANINI_SOURCE_DIR}/cmake")

project(Panini)

# panini requires at least C++17

set(CMAKE_CXX_STANDARD 17)

# google test dependency

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# prevent overriding the parent project's compiler and linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# panini target

add_library(Panini INTERFACE)
target_include_directories(Panini INTERFACE include/)

# tests

add_subdirectory(source/tests)