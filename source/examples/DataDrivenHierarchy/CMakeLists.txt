add_executable(
	DataDrivenHierarchy
	DataDrivenHierarchy.cpp
)
target_link_libraries(
	DataDrivenHierarchy
	panini
)
add_custom_command(
	TARGET DataDrivenHierarchy
	POST_BUILD COMMAND
		${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/Hierarchy.ini
		${CMAKE_CURRENT_BINARY_DIR}
)
set_target_properties(DataDrivenHierarchy PROPERTIES FOLDER "Panini/Examples")
