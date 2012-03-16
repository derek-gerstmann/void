remove_definitions( -DVD_BUILDING_FRAMEWORK )

add_library( VdExternal ${VD_LIBRARY_TYPE} ${VD_EXT_SRC} )
add_dependencies( VdExternal ${VD_EXT_SRC} )
target_link_libraries( VdExternal ${VD_EXT_LIB} )
message(STATUS "ICONV: '${VD_EXT_ICONV_LIB}'"')
message(STATUS "Configured: Void External Libraries '${VD_EXT_LIB}'")
