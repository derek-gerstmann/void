# cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
# project( vdsymex )

# file( GLOB VD_TOOLS_SYMEX_SRC   			   ${VD_TOOLS_DIR}/symex/*.cpp)
# set( VD_CORE_HASHING_SRC		    	           ${VD_CORE_DIR}/hashing.cpp )

# include_directories(
#     ${VD_TOOLS_DIR}/symex
#     ${VD_CORE_DIR}
# )

# add_executable( vdsymex 
#     ${VD_TOOLS_SYMEX_SRC} 
#     ${VD_CORE_HASHING_SRC} 
# )

# add_dependencies( vdsymex 
#     ${VD_ALL_SRC} ${VD_ALL_INC} 
#     ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} 
#     ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC} )
    
# target_link_libraries( vdsymex 
#     VdExternal 
#     VdPlatform 
#     ${VD_EXT_BOOST_LIB}
# )

# message(STATUS "Configured: SymEx")
