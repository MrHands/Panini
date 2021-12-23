add_executable(
	HelloWorld
	HelloWorld.cpp
)
target_link_libraries(
	HelloWorld
	panini
)
set_target_properties(HelloWorld PROPERTIES FOLDER "Panini/Examples")