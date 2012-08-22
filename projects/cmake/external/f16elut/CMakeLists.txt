cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdf16lut )

set( VD_EXT_HALF_ELUT_SRC ${VD_EXT_DIR}/source/half/src/eLut.cpp )
include_directories( ${VD_EXT_DIR}/source/half )
add_executable( vdf16lut ${VD_EXT_HALF_ELUT_SRC} )

message(STATUS "Configured: VdHalfELut")
