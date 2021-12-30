add_executable(
	PaniniTests
	TestsBraces.cpp
	TestsCommentBlock.cpp
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
	panini
	gtest_main
)
set_target_properties(PaniniTests PROPERTIES FOLDER "Panini/Tests")

include(GoogleTest)
gtest_discover_tests(PaniniTests)