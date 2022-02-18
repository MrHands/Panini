file(
	GLOB PANINI_SOURCE_TESTS
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/source/tests/*.cpp
)

add_executable(
	PaniniTests
	${PANINI_SOURCE_TESTS}
)
target_link_libraries(
	PaniniTests
	panini
	gtest_main
)
set_target_properties(PaniniTests PROPERTIES FOLDER "Panini/Tests")

include(GoogleTest)
gtest_discover_tests(PaniniTests)
