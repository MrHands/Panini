cmake_minimum_required(VERSION 3.18)

# panini requires at least C++17

project(
	panini
	VERSION 1.0.0
	LANGUAGES CXX
	DESCRIPTION "Header-only library for generating C++, written in C++17"
	HOMEPAGE_URL "https://github.com/MrHands/Panini"
)
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

set(PANINI_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include")
set(PANINI_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")

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

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
	${PROJECT_NAME}
	INTERFACE
		$<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features(${PROJECT_NAME}	INTERFACE cxx_std_17)

# reference project to show headers

if(PANINI_BUILDING)
	file(
		GLOB
		PANINI_INCLUDES
		${${PROJECT_NAME}_SOURCE_DIR}/include/*.hpp
	)
	file(
		GLOB
		PANINI_INCLUDES_COMMANDS
		${${PROJECT_NAME}_SOURCE_DIR}/include/commands/*.hpp
	)
	file(
		GLOB
		PANINI_INCLUDES_WRITERS
		${${PROJECT_NAME}_SOURCE_DIR}/include/writers/*.hpp
	)

	add_executable(
		HeaderOnlyLibraryProject
		source/HeaderOnlyLibraryProject.cpp
		${PANINI_INCLUDES}
		${PANINI_INCLUDES_COMMANDS}
		${PANINI_INCLUDES_WRITERS}
	)
	target_link_libraries(
		HeaderOnlyLibraryProject
		panini
	)
	source_group("include" FILES ${PANINI_INCLUDES})
	source_group("include/commands" FILES ${PANINI_INCLUDES_COMMANDS})
	source_group("include/writers" FILES ${PANINI_INCLUDES_WRITERS})
endif()

# tests

if(PANINI_BUILD_TESTS)
	# google test dependency

	include(FetchContent)
	FetchContent_Declare(
		googletest
		URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
	)
	# prevent overriding the parent project's compiler and linker settings
	set(
		gtest_force_shared_crt ON
		CACHE
		BOOL
		""
		FORCE
	)
	FetchContent_MakeAvailable(googletest)

	enable_testing()
	add_subdirectory(source/tests)
endif()

# examples

if(PANINI_BUILD_EXAMPLES)
	add_subdirectory(source/examples)
endif()

# install

if(PANINI_INSTALL)
	# select targets to install

	install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}_Targets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)

	# compare major version against package

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		"${PROJECT_NAME}ConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	# generate the configuration file for the installed package

	configure_package_config_file(
		"${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		INSTALL_DESTINATION
		${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
	)

	# install targets

	install(
		EXPORT ${PROJECT_NAME}_Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
	)
	install(
		FILES
			"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
			"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		DESTINATION
			${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
	)
	install(
		DIRECTORY ${PROJECT_SOURCE_DIR}/include
		DESTINATION include
	)
endif()