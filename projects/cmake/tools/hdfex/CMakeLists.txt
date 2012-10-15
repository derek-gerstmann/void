cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdhdfex )

set( VD_EXT_ZLIB_DIR					${VD_EXT_SYS_DIR}/zlib/latest )
set( VD_EXT_ZLIB_INCLUDE_PATH			${VD_EXT_ZLIB_DIR}/include )
set( VD_EXT_ZLIB_LIB_PATH				${VD_EXT_ZLIB_DIR}/lib )
include_directories(					${VD_EXT_ZLIB_INCLUDE_PATH} )
file( GLOB VD_EXT_ZLIB_LIBS				${VD_EXT_ZLIB_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

set( VD_EXT_SZIP_DIR 					${VD_EXT_SYS_DIR}/szip/latest )
set( VD_EXT_SZIP_INCLUDE_PATH			${VD_EXT_SZIP_DIR}/include )
set( VD_EXT_SZIP_LIB_PATH				${VD_EXT_SZIP_DIR}/lib )
file( GLOB VD_EXT_SZIP_LIBS				${VD_EXT_SZIP_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    				${VD_EXT_SZIP_INCLUDE_PATH} )

if(VOID_USE_PARALLEL_HDF5)
	set( VD_EXT_HDF5_DIR 				${VD_EXT_SYS_DIR}/phdf5/latest )
else()
	set( VD_EXT_HDF5_DIR				${VD_EXT_SYS_DIR}/hdf5/latest )
endif()

set( VD_EXT_HDF5_INCLUDE_PATH			${VD_EXT_HDF5_DIR}/include )
set( VD_EXT_HDF5_LIB_PATH				${VD_EXT_HDF5_DIR}/lib )
file( GLOB VD_EXT_HDF5_LIBS				${VD_EXT_HDF5_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    				${VD_EXT_HDF5_INCLUDE_PATH} )
include_directories(    				${VD_EXT_HDF5_INCLUDE_PATH}/cpp )

set( VD_EXT_BOOST_DIR					${VD_EXT_SYS_DIR}/boost/latest )
set( VD_EXT_BOOST_INCLUDE_PATH			${VD_EXT_BOOST_DIR}/include )
set( VD_EXT_BOOST_LIB_PATH				${VD_EXT_BOOST_DIR}/lib )
include_directories(					${VD_EXT_BOOST_INCLUDE_PATH} )
include_directories(					${VD_EXT_BOOST_INCLUDE_PATH}/boost )
file( GLOB VD_EXT_BOOST_LIBS			${VD_EXT_BOOST_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

file( GLOB VD_TOOLS_HDFEX_SRC  
	${VD_TOOLS_DIR}/hdfex/*.cpp)

include_directories(
    ${VD_TOOLS_DIR}/hdfex
)

add_executable( vdhdfex 
    ${VD_TOOLS_HDFEX_SRC} 
)

target_link_libraries( vdhdfex 
    ${VD_EXT_BOOST_LIBS}
    ${VD_EXT_HDF5_LIBS} 
    ${VD_EXT_SZIP_LIBS} 
    ${VD_EXT_ZLIB_LIBS}
    ${VD_PLATFORM_LIBS}
 )

message(STATUS "Configured: vdhdfex")
