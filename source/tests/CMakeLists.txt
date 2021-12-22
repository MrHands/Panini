enable_testing()

include_directories("${PANINI_SOURCE_DIR}/include")

add_executable(
	PaniniTests
	TestsBraces.cpp
	TestsCommentLine.cpp
	TestsCompareWriter.cpp
	TestsConsoleWriter.cpp
	TestsFileWriter.cpp
	TestsInclude.cpp
	TestsIndentation.cpp
	TestsMain.cpp
	TestsNewLine.cpp
	TestsScope.cpp
	TestsStringWriter.cpp
)
target_link_libraries(
	PaniniTests
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(PaniniTests)