add_executable(
	GenerateXml
	GenerateXml.cpp
)
target_link_libraries(
	GenerateXml
	panini
)
set_target_properties(GenerateXml PROPERTIES FOLDER "Panini/Examples")