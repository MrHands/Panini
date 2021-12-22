cmake_minimum_required(VERSION 3.18)

set(PANINI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
message(DEBUG "${PANINI_SOURCE_DIR}")

list(APPEND CMAKE_MODULE_PATH "${PANINI_SOURCE_DIR}/cmake")
# find_package(GoogleTest REQUIRED)

project(Panini)

add_subdirectory(source/tests)