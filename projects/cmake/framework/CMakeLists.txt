cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdFramework )

if(VOID_USE_EXTRACTED_SYMBOLS)

    set( VD_SYMEX_FILELIST 
            ${VD_ALL_SRC} 
            ${VD_ALL_INC} 
            ${VD_ALL_OPT_SRC} 
            ${VD_ALL_OPT_INC} 
            ${VD_PLATFORM_SRC} )

    file(MAKE_DIRECTORY ${VD_EXTRACTED_DIR} )
    
    extract_symbols(
        ${VD_SYMBOLS_H} 
        VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED 
        VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
        Global
        ${VD_SYMEX_FILELIST})

    add_custom_target(VdSymExHeader ALL DEPENDS ${VD_ALL_SRC} ${VD_ALL_INC} )
    set(VD_ALL_INC ${VD_ALL_INC} ${VD_SYMBOLS_H})
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )

endif()

if(EXISTS "${VD_SYMBOLS_H}")
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )
    add_definitions( -DVD_EXTRACTED_SYMBOLS_EXIST=1 )
    message(STATUS "Using Extracted Symbols: ${VD_SYMBOLS_H}")
endif()

add_definitions( -DVD_BUILDING_FRAMEWORK=1 )
add_library( VdFramework ${VD_LIBRARY_TYPE} ${VD_ALL_SRC} ${VD_ALL_INC} )
message(STATUS "Configured Framework: Void v${VOID_VERSION_STRING}")

install(DIRECTORY ${VD_SRC_DIR}/ DESTINATION include FILES_MATCHING PATTERN "*.h")