cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
if(VOID_USE_HDF5)
project( vdhdfex )

file( GLOB
      VD_TOOLS_HDFEX_SRC  
      ${VD_TOOLS_DIR}/hdfex/*.cpp)

include_directories(
    ${VD_TOOLS_DIR}/hdfex
)

add_executable( vdhdfex 
    ${VD_TOOLS_HDFEX_SRC} 
)

target_link_libraries( vdhdfex 
    ${VD_EXT_BOOST_LIB}
    ${VD_EXT_HDF5_LIB} 
    ${VD_EXT_SZIP_LIB} 
    ${VD_EXT_ZLIB_LIB}
 )

message(STATUS "Configured: vdhdfex")
endif()
