include(FindPackageHandleStandardArgs)

find_path(GTEST_INCLUDE_DIR
	NAMES gtest/gtest.h
	HINTS "${GTEST_PATH}/googletest/include")

find_package_handle_standard_args(GoogleTest
	DEFAULT_MSG
	GTEST_LIBRARY
	GTEST_INCLUDE_DIR)