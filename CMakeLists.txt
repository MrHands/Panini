cmake_minimum_required(VERSION 3.19)

# panini requires at least C++17

project(
	panini
	VERSION 1.2.0
	LANGUAGES CXX
	DESCRIPTION "Header-only library for generating C++, written in C++17"
	HOMEPAGE_URL "https://github.com/MrHands/Panini"
)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
	PANINI_BUILD_DOCS
	"Build the documentation with Doxygen"
	${PANINI_BUILDING}
)
option(
	PANINI_INSTALL
	"Install CMake targets during the install step"
	${PANINI_BUILDING}
)

# target

file(
	GLOB PANINI_INCLUDES
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/include/*.hpp
)
file(
	GLOB PANINI_INCLUDES_COMMANDS
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/include/commands/*.hpp
)
file(
	GLOB PANINI_INCLUDES_DATA
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/include/data/*.hpp
)
file(
	GLOB PANINI_INCLUDES_WRITERS
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/include/writers/*.hpp
)

add_library(
	${PROJECT_NAME} INTERFACE
	${PANINI_INCLUDES}
	${PANINI_INCLUDES_COMMANDS}
	${PANINI_INCLUDES_DATA}
	${PANINI_INCLUDES_WRITERS}
)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
		FOLDER "Panini"
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
)

source_group("include" FILES ${PANINI_INCLUDES})
source_group("include/commands" FILES ${PANINI_INCLUDES_COMMANDS})
source_group("include/data" FILES ${PANINI_INCLUDES_DATA})
source_group("include/writers" FILES ${PANINI_INCLUDES_WRITERS})

target_include_directories(
	${PROJECT_NAME} INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_compile_features(
	${PROJECT_NAME} INTERFACE
		cxx_std_17
)

# fix for Visual Studio 2017

if (MSVC_VERSION LESS_EQUAL "1916")
	target_compile_definitions(
		${PROJECT_NAME} INTERFACE
			_HAS_CXX17=1
	)
endif()

# documentation

if(PANINI_BUILD_DOCS)
	set(DOXYGEN_SKIP_DOT TRUE)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		add_subdirectory(source/documentation)
	endif()
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

	# generate version file

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.hpp.in
		${CMAKE_CURRENT_SOURCE_DIR}/include/Version.hpp
		@ONLY
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
