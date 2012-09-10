cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdFramework )

if(EXISTS "${VD_SYMBOLS_H}")
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )
    add_definitions( -DVD_EXTRACTED_SYMBOLS_EXIST=1 )
    message(STATUS "Using Extracted Symbols: ${VD_SYMBOLS_H}")
endif()

add_definitions( -DVD_BUILDING_FRAMEWORK=1 )
add_library( VdFramework ${VD_LIBRARY_TYPE} ${VD_ALL_SRC} ${VD_ALL_INC} )

if(VOID_USE_EXTRACTED_SYMBOLS)
    add_dependencies( VdFramework VdSymExHeader )
endif()

message(STATUS "Configured Framework: Void v${VOID_VERSION_STRING}")

install(DIRECTORY ${VD_SRC_DIR}/ DESTINATION include FILES_MATCHING PATTERN "*.h")