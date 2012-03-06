
if(VOID_USE_EXTRACTED_SYMBOLS)

    set( VD_SYMEX_FILELIST ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} ${VD_PLATFORM_SRC} )

    file(MAKE_DIRECTORY ${VD_EXTRACTED_DIR} )
    
    extract_symbols(
        ${VD_SYMBOLS_H} 
        VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED 
        VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
        Global
        ${VD_SYMEX_FILELIST})

    add_custom_target(VdSymExHeader ALL DEPENDS ${VD_SYMBOLS_H} ${VD_ALL_SRC} ${VD_ALL_INC} )

endif()


if(EXISTS ${VD_SYMBOLS_H})
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )
    add_definitions( -DVD_EXTRACTED_SYMBOLS_EXIST=1 )
endif()
    
add_library( VdFramework ${VD_LIBRARY_TYPE} ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_SYMBOLS_H} )
add_dependencies( VdFramework ${VD_ALL_SRC} ${VD_SYMBOLS_H} )
message(STATUS "Configured: Void Framework ${VOID_VERSION_STRING} ${CMAKE_BUILD_TYPE}")
