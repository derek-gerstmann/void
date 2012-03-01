file( GLOB VD_TOOLS_SUBDIRS ./tools/* )
foreach( entry ${VD_TOOLS_SUBDIRS} )
	if( IS_DIRECTORY ${entry} )
		message(STATUS "Adding tool '${entry}' ...")
		add_subdirectory( ${entry} )
	endif()
endforeach()

