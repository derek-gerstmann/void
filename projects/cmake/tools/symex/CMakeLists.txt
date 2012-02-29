cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project( VdSymEx )

file( GLOB VD_TOOLS_SYMEX_SRC  	${VD_TOOLS_DIR}/symex/*.c ${VD_TOOLS_DIR}/symex/*.cpp)
set( VD_CORE_HASHING_SRC		${VD_SRC_DIR}/core/hashing.cpp )

include_directories(
	${VD_TOOLS_DIR}/symex
)

add_executable( VdSymEx 
	${VD_TOOLS_SYMEX_SRC} ${VD_CORE_HASHING_SRC}
)

target_link_libraries( VdSymEx 
	VdExternal 
)

add_custom_target( VdExtractSymbols ALL DEPENDS 
	${VD_ALL_SRC} ${VD_ALL_INC} ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC} ${VD_SYMBOLS_H} )

message(STATUS "Configured SymEx ...")
