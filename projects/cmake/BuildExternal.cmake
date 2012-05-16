file( GLOB VD_EXT_TOOLS_SUBDIRS ./external/* )
foreach( entry ${VD_EXT_TOOLS_SUBDIRS} )
    if( IS_DIRECTORY ${entry} )
        message(STATUS "Adding External '${entry}' ...")
        add_subdirectory( ${entry} )
    endif()
endforeach()

