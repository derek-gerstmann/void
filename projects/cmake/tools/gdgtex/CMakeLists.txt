cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdgdex )

file( GLOB VD_TOOLS_GDGTEX_SRC 
    ${VD_TOOLS_DIR}/gdgtex/*.c 
    ${VD_TOOLS_DIR}/gdgtex/*.cpp
)

include_directories( ${VD_TOOLS_DIR}/gdgtex )

add_executable( vdgdex ${VD_TOOLS_GDGTEX_SRC} )

message(STATUS "Configured: GdgtEx")
