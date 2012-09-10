cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdgdex )

file( GLOB VD_TOOLS_GDGTEX_SRC 
    ${VD_TOOLS_DIR}/gdgtex/*.c 
    ${VD_TOOLS_DIR}/gdgtex/*.cpp
)

include_directories( ${VD_TOOLS_DIR}/gdgtex )

if(VOID_USE_HDF5)
	add_executable( vdgdex ${VD_TOOLS_GDGTEX_SRC} )
	target_link_libraries( vdgdex ${VD_EXT_HDF5_LIB} ${VD_EXT_SZIP_LIB} ${VD_EXT_ZLIB_LIB} )
else()
	add_executable( vdgdex ${VD_TOOLS_GDGTEX_SRC} )
endif()

message(STATUS "Configured: GdgtEx")
