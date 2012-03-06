cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdSymEx )
set( VD_EXT_BOOST_DIR               ${VD_EXT_DIR}/boost )

include_directories(                ${VD_EXT_SIMD_DIR}/include )
include_directories(                ${VD_EXT_BOOST_DIR}/include )

file( GLOB VD_EXT_BOOST_LIB         ${VD_EXT_BOOST_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
file( GLOB VD_TOOLS_SYMEX_SRC   	${VD_TOOLS_DIR}/symex/*.c ${VD_TOOLS_DIR}/symex/*.cpp)
set( VD_CORE_HASHING_SRC		    ${VD_CORE_DIR}/hashing.cpp )

include_directories(
	${VD_TOOLS_DIR}/symex
    ${VD_CORE_DIR}
)

add_executable( VdSymEx 
    ${VD_TOOLS_SYMEX_SRC} 
    ${VD_CORE_HASHING_SRC} 
)

target_link_libraries( VdSymEx 
	${VD_EXT_BOOST_LIB} 
)

add_dependencies( VdSymEx 
    ${VD_ALL_SRC} ${VD_ALL_INC} 
    ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} 
    ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC} )
    
message(STATUS "Configured SymEx ...")
