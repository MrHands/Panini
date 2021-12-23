file(
	GLOB
	PANINI_DOXYGEN_SOURCES
	${PANINI_SOURCE_DIR}/include/**/*.hpp
)

foreach(path IN LISTS PANINI_DOXYGEN_SOURCES)
	set(
		PANINI_DOXYGEN_INPUT
		"${PANINI_DOXYGEN_INPUT} \\\n\"${PANINI_SOURCE_DIR}/${path}\""
	)
endforeach()

configure_file(
	Doxyfile.in
	Doxyfile
	@ONLY
)

add_custom_target(
	documentation
	ALL "${DOXYGEN_EXECUTABLE}"
	WORKING_DIRECTORY "${PANINI_SOURCE_DIR}/documentation"
	COMMENT "Generating HTML documentation..." VERBATIM
)

set_target_properties(
	documentation
	PROPERTIES FOLDER "Panini"
)