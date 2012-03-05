cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdHalfToFloat )

set( VD_EXT_HALF_TO_FLOAT_SRC ${VD_EXT_DIR}/half/src/toFloat.cpp )
include_directories( ${VD_EXT_DIR}/half )
add_executable( VdHalfToFloat ${VD_EXT_HALF_TO_FLOAT_SRC} )

message(STATUS "Configured VdHalfToFloat ...")
