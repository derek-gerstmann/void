cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( vdhdfex )

execute_process(COMMAND 		module load zlib/latest)
set( VD_EXT_ZLIB_DIR			$ENV{BLDR_ZLIB_BASE_PATH} )
file( GLOB VD_EXT_ZLIB_LIBS		$ENV{BLDR_ZLIB_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    		$ENV{BLDR_ZLIB_INCLUDE_PATH} )

execute_process(COMMAND 		module load szip/latest)
file( GLOB VD_EXT_SZIP_LIBS		$ENV{BLDR_SZIP_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    		$ENV{BLDR_SZIP_INCLUDE_PATH} )

execute_process(COMMAND 		module load hdf5/latest)
file( GLOB VD_EXT_HDF5_LIBS		$ENV{BLDR_HDF5_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    		$ENV{BLDR_HDF5_INCLUDE_PATH} )
include_directories(    		$ENV{BLDR_HDF5_INCLUDE_PATH}/cpp )

execute_process(COMMAND 		module load boost/latest)
file( GLOB VD_EXT_BOOST_LIBS	$ENV{BLDR_BOOST_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
include_directories(    		$ENV{BLDR_BOOST_INCLUDE_PATH} )
include_directories(    		$ENV{BLDR_BOOST_INCLUDE_PATH}/boost )

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
 )

message(STATUS "Configured: vdhdfex")
