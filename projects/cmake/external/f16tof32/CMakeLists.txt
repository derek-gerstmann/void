cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdf16tof32 )

set( VD_EXT_HALF_TO_FLOAT_SRC ${VD_EXT_DIR}/source/half/latest/src/toFloat.cpp )
include_directories( ${VD_EXT_DIR}/source/half/latest/include )
add_executable( vdf16tof32 ${VD_EXT_HALF_TO_FLOAT_SRC} )

message(STATUS "Configured: vdf16tof32")
