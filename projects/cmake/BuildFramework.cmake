include( UseFramework )
include(${VD_TOOLS_DIR}/symex)

if(VOID_USE_EXTRACTED_SYMBOLS)
    set( VD_SYMEX_FILELIST ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} ${VD_PLATFORM_SRC} )
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=VD_ON )
    add_definitions( -DVD_EXTRACTED_SYMBOLS_H="${VD_SYMBOLS_H}" )
    add_custom_command ( 
        OUTPUT ${VD_SYMBOLS_H}
        COMMENT "Extracting Static Symbols and Strings to ${VD_SYMBOLS_H} ..."
        COMMAND mkdir -p ${VD_EXTRACTED_DIR}
        COMMAND VdSymEx ${VD_SYMEX_FILELIST} -i "core/core.h" -i "core/symbol.h" -i "constants/constants.h" -n extracted -n Symbols -o ${VD_SYMBOLS_H}
        DEPENDS ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC}
    )
endif()

add_library( VdFramework ${VD_LIBRARY_TYPE} ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_SYMBOLS_H} )
add_dependencies( VdFramework ${VD_ALL_SRC} ${VD_SYMBOLS_H} )
message(STATUS "Configured: Void Framework ${VOID_VERSION_STRING} ${CMAKE_BUILD_TYPE}")
