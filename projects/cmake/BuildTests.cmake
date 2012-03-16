remove_definitions( -DVD_BUILDING_FRAMEWORK )

file( GLOB VD_TESTS_SUBDIRS ./tests/* )
foreach( entry ${VD_TESTS_SUBDIRS} )
	if( IS_DIRECTORY ${entry} )
		message(STATUS "Adding test '${entry}' ...")
		add_subdirectory( ${entry} )
	endif()
endforeach( entry ${VD_TEST_DIR} )

