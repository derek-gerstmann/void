remove_definitions( -DVD_BUILDING_FRAMEWORK )

file( GLOB VD_APP_SUBDIRS ./apps/* )
foreach( entry ${VD_APP_SUBDIRS} )
	if( IS_DIRECTORY ${entry} )
#		message(STATUS "Adding Application '${entry}' ...")
		add_subdirectory( ${entry} )
	endif()
endforeach()

