cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdGsEx )

file( GLOB VD_TOOLS_GS2_SRC  	${VD_TOOLS_DIR}/gsex/*.c ${VD_TOOLS_DIR}/gsex/*.cpp)

include_directories( ${VD_TOOLS_DIR}/gsex )

add_executable( VdGdgtEx ${VD_TOOLS_GS2_SRC} )

message(STATUS "Configured GsEx ...")
