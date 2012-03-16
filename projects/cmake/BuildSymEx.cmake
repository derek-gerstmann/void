remove_definitions( -DVD_BUILDING_FRAMEWORK )

file( GLOB VD_TOOLS_SYMEX_SRC   ${VD_TOOLS_DIR}/symex/*.c ${VD_TOOLS_DIR}/symex/*.cpp)
set( VD_CORE_HASHING_SRC        ${VD_CORE_DIR}/hashing.cpp )

include_directories(
    ${VD_TOOLS_DIR}/symex
    ${VD_CORE_DIR}
)

add_executable( VdSymEx 
    ${VD_TOOLS_SYMEX_SRC} 
    ${VD_CORE_HASHING_SRC} 
)

target_link_libraries( VdSymEx 
    VdExternal 
)

add_dependencies( VdSymEx 
    ${VD_ALL_SRC} ${VD_ALL_INC} 
    ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} 
    ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC})
    
add_custom_target( VdExtractSymbols ALL DEPENDS 
    ${VD_ALL_SRC} ${VD_ALL_INC} 
    ${VD_PLATFORM_SRC} 
    ${VD_TOOLS_SYMEX_SRC} )

message(STATUS "Configured SymEx ...")
