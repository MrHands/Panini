file(
	GLOB_RECURSE
	PANINI_DOXYGEN_SOURCES
	CONFIGURE_DEPENDS
	${${PROJECT_NAME}_SOURCE_DIR}/include/*.hpp
)

foreach(path IN LISTS PANINI_DOXYGEN_SOURCES)
	set(
		PANINI_DOXYGEN_INPUT
		"${PANINI_DOXYGEN_INPUT} \\\n\"${path}\""
	)
endforeach()

# add readme

set(
	PANINI_DOXYGEN_INPUT
	"${PANINI_DOXYGEN_INPUT} \\\n\"${${PROJECT_NAME}_SOURCE_DIR}/README.md\""
)

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_target(
	documentation
	ALL "${DOXYGEN_EXECUTABLE}"
	COMMENT "Generating HTML documentation..." VERBATIM
)

set_target_properties(
	documentation
	PROPERTIES FOLDER "Panini"
)
