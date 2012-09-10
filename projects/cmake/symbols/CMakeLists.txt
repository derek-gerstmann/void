cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdsymex )

file( GLOB VD_TOOLS_SYMEX_SRC 	${VD_TOOLS_DIR}/symex/*.cpp)
set( VD_CORE_HASHING_SRC	      ${VD_CORE_DIR}/hashing.cpp )

include_directories(
    ${VD_TOOLS_DIR}/symex
    ${VD_CORE_DIR}
)

add_executable( vdsymex 
    ${VD_TOOLS_SYMEX_SRC} 
    ${VD_CORE_HASHING_SRC} 
)
    
target_link_libraries( vdsymex 
    VdExternal 
    VdPlatform 
    ${VD_EXT_BOOST_LIB}
)

message(STATUS "Configured: SymEx")

if(VOID_USE_EXTRACTED_SYMBOLS)

    set( VD_SYMEX_FILELIST 
            ${VD_ALL_SRC} 
            ${VD_ALL_INC} 
            ${VD_PLATFORM_SRC} )

    file(MAKE_DIRECTORY ${VD_EXTRACTED_DIR} )
    
    extract_symbols(
        ${VD_SYMBOLS_H} 
        VD_CONSTANTS_EXTRACTED_GLOBAL_SYMBOLS_INCLUDED 
        VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
        Global
        ${VD_SYMEX_FILELIST})

    add_custom_target(VdSymExHeader ALL DEPENDS ${VD_ALL_SRC} ${VD_ALL_INC} )
    set_source_files_properties(${VD_SYMBOLS_H} PROPERTIES HEADER_FILE_ONLY TRUE)
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )
endif()

add_library( VdSymbols ${VD_LIBRARY_TYPE} ${VD_SYMBOLS_H} ${VD_SYMBOLS_CPP} )
message(STATUS "Configured: VdSymbols")
