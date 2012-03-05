cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdHalfELut )

set( VD_EXT_HALF_ELUT_SRC ${VD_EXT_DIR}/half/src/eLut.cpp )
include_directories( ${VD_EXT_DIR}/half )
add_executable( VdHalfELut ${VD_EXT_HALF_ELUT_SRC} )

message(STATUS "Configured VdHalfELut ...")
