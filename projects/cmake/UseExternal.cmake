set( VD_EXT_SIMD_DIR				${VD_EXT_DIR}/simd )
set( VD_EXT_ICONV_DIR				${VD_EXT_DIR}/iconv )
set( VD_EXT_BOOST_DIR				${VD_EXT_DIR}/boost )

include_directories(				${VD_EXT_SIMD_DIR}/include )
include_directories(				${VD_EXT_ICONV_DIR}/include )
include_directories(				${VD_EXT_BOOST_DIR}/include )

file( GLOB VD_EXT_ICONV_LIB			${VD_EXT_ICONV_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
file( GLOB VD_EXT_BOOST_LIB			${VD_EXT_BOOST_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
set( VD_EXT_LIB 				    ${VD_EXT_BOOST_LIB} ${VD_EXT_ICONV_LIB} ${VD_EXT_LIB} )

if(VOID_USE_HALF)
	set( VD_EXT_HALF_DIR			${VD_EXT_DIR}/half )
	file( GLOB VD_EXT_HALF_SRC		${VD_EXT_HALF_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_HALF_DIR}/include)
	set( VD_EXT_SRC 				${VD_EXT_HALF_SRC} ${VD_EXT_SRC} )
endif()

if(VOID_USE_ASMLIB)
	set( VD_EXT_ALIB_DIR			${VD_EXT_DIR}/alib )
	file( GLOB VD_EXT_ALIB_LIB		${VD_EXT_ALIB_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	include_directories(			${VD_EXT_ALIB_DIR}/include)
	set( VD_EXT_LIB 				${VD_EXT_ALIB_LIB} ${VD_EXT_LIB} )
endif()

if(VOID_USE_OPENGL)
	
	find_package(OpenGL 			REQUIRED)
	
	set( VD_EXT_FT_DIR				${VD_EXT_DIR}/ft )
	set( VD_EXT_FTGL_DIR			${VD_EXT_DIR}/ftgl )
	set( VD_EXT_GLEW_DIR			${VD_EXT_DIR}/glew )
	set( VD_EXT_GLSW_DIR			${VD_EXT_DIR}/glsw )

	file( GLOB VD_EXT_GLSW_SRC 	 	${VD_EXT_GLSW_DIR}/src/*.c )
	file( GLOB VD_EXT_FTGL_SRC 	 	${VD_EXT_FTGL_DIR}/src/*.c )
	file( GLOB VD_EXT_GLEW_LIB		${VD_EXT_GLEW_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	file( GLOB VD_EXT_FT_LIB 	 	${VD_EXT_FT_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	set( VD_EXT_SRC 				${VD_EXT_FTGL_SRC} ${VD_EXT_GLSW_SRC} ${VD_EXT_SRC} )

	include_directories(
		${OPENGL_INCLUDE_DIRS}
		${VD_EXT_FT_DIR}/include
		${VD_EXT_FTGL_DIR}/include
		${VD_EXT_GLEW_DIR}/src
		${VD_EXT_GLEW_DIR}/include
		${VD_EXT_GLEW_DIR}/include/GL
		${VD_EXT_GLSW_DIR}/src)

	set( VD_EXT_LIB 				${OPENGL_LIBRARIES} ${VD_EXT_LIB} )
	set( VD_EXT_LIB 				${VD_EXT_FT_LIB} ${VD_EXT_GLEW_LIB} ${VD_EXT_LIB} )

	if(VOID_USE_GLUT)
		if(VD_SYSTEM_LNX)
			set( VD_EXT_GLUT_DIR 	${VD_EXT_DIR}/glut )
			set( GLUT_INCLUDE_DIRS	${VD_EXT_GLUT_DIR}/include ${VD_EXT_GLUT_DIR}/include/GL )
			file( GLOB GLUT_LIBRARY ${VD_EXT_GLUT_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
		else()
			find_package(GLUT 		REQUIRED)
		endif()

		include_directories(		${GLUT_INCLUDE_DIRS} )
		set( VD_EXT_LIB 			${GLUT_LIBRARY} ${VD_EXT_LIB} )
	endif()
		
endif()

if(VOID_BUILD_TESTS)
	set( VD_EXT_GTEST_DIR			${VD_EXT_DIR}/gtest )
	file( GLOB VD_EXT_GTEST_LIB	 	${VD_EXT_GTEST_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
endif()

if(VOID_USE_GWEN)
	set( VD_EXT_GWEN_DIR			${VD_EXT_DIR}/gwen )
	file( GLOB VD_EXT_GWEN_LIB 	 	${VD_EXT_GWEN_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )	
	include_directories(			${VD_EXT_GWEN_DIR}/include )
	set( VD_EXT_LIB 				${VD_EXT_GWEN_LIB} ${VD_EXT_LIB} )
endif()

if(VOID_USE_JSON)
	set( VD_EXT_JSON_DIR			${VD_EXT_DIR}/json )
	file( GLOB VD_EXT_JSON_SRC		${VD_EXT_JSON_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_JSON_DIR}/src  ${VD_EXT_JSON_DIR}/include )
	set( VD_EXT_SRC 				${VD_EXT_JSON_SRC} ${VD_EXT_SRC} )
endif()

if(VOID_USE_MSGPACK)
	set( VD_EXT_MSGPACK_DIR			${VD_EXT_DIR}/msgpack )
	file( GLOB VD_EXT_MSGPACK_LIB	${VD_EXT_MSGPACK_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )	
	include_directories(    		${VD_EXT_MSGPACK_DIR}/include )
	set( VD_EXT_LIB 				${VD_EXT_MSGPACK_LIB} ${VD_EXT_LIB} )
endif()

if(VOID_USE_HDF5)
	set( VD_EXT_HDF5_DIR			${VD_EXT_DIR}/hdf )
	file( GLOB VD_EXT_HDF5_LIB		${VD_EXT_HDF5_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )	
	include_directories(    		${VD_EXT_HDF5_DIR}/include )
	set( VD_EXT_LIB 				${VD_EXT_HDF5_LIB} ${VD_EXT_LIB} )
endif()

if(VOID_USE_OIIO)
	set( VD_EXT_OIIO_DIR			${VD_EXT_DIR}/oiio )
	set( VD_EXT_PNG_DIR				${VD_EXT_DIR}/png )
	set( VD_EXT_JPEG_DIR			${VD_EXT_DIR}/jpeg )
	set( VD_EXT_EXR_DIR				${VD_EXT_DIR}/exr )

	file( GLOB VD_EXT_OIIO_LIB		${VD_EXT_OIIO_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	file( GLOB VD_EXT_PNG_LIB		${VD_EXT_PNG_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	file( GLOB VD_EXT_EXR_LIB 	 	${VD_EXT_EXR_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	file( GLOB VD_EXT_JPEG_LIB 	 	${VD_EXT_JPEG_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	
	include_directories(
		${VD_EXT_OIIO_DIR}/include
		${VD_EXT_EXR_DIR}/include
		${VD_EXT_EXR_DIR}/include/OpenEXR
		${VD_EXT_PNG_DIR}/include
		${VD_EXT_JPEG_DIR}/include
		${VD_EXT_JPEG_DIR}/include/jpeg )
	
	set( VD_EXT_LIB 
		${VD_EXT_EXR_LIB}
		${VD_EXT_PNG_LIB}
		${VD_EXT_JPEG_LIB}
		${VD_EXT_OIIO_LIB}
		${VD_EXT_LIB} )
endif()

if(VOID_USE_SFML)
	set( VD_EXT_SFML_DIR			${VD_EXT_DIR}/sfml )
	file( GLOB VD_EXT_SFML_LIB 	 	${VD_EXT_SFML_DIR}/lib/${VD_OS_NAME}/*-s.${VD_STATIC_LIB_EXT} )
	include_directories(			${VD_EXT_SFML_DIR}/include ${VD_EXT_SFML_DIR}/include/SFML )
	set( VD_EXT_LIB 				${VD_EXT_SFML_LIB} ${VD_EXT_LIB} )	
endif()

if(VOID_USE_SKIA)

	set( VD_EXT_SKIA_DIR			${VD_EXT_DIR}/skia )
	file( GLOB VD_EXT_SKIA_LIB 		${VD_EXT_SKIA_DIR}/lib/${VD_OS_NAME}/*.${VD_STATIC_LIB_EXT} )
	file( GLOB VD_EXT_SKIA_SRC 		${VD_EXT_SKIA_DIR}/src/${VD_OS_NAME}/*.h ${VD_EXT_SKIA_DIR}/src/${VD_OS_NAME}/*.cpp ${VD_EXT_SKIA_DIR}/src/${VD_OS_NAME}/*.mm )	
	set( VD_EXT_LIB     			${VD_EXT_SKIA_LIB} ${VD_EXT_LIB} )
	set( VD_EXT_SRC 				${VD_EXT_SKIA_SRC} ${VD_EXT_SRC} )
	
	if(APPLE)
		add_definitions(-DSK_BUILD_FOR_MAC=1)
	endif()

	include_directories(
		${VD_EXT_SKIA_DIR}/include
		${VD_EXT_SKIA_DIR}/include/animator
		${VD_EXT_SKIA_DIR}/include/config
		${VD_EXT_SKIA_DIR}/include/core
		${VD_EXT_SKIA_DIR}/include/device
		${VD_EXT_SKIA_DIR}/include/effects
		${VD_EXT_SKIA_DIR}/include/gpu
		${VD_EXT_SKIA_DIR}/include/images
		${VD_EXT_SKIA_DIR}/include/pdf
		${VD_EXT_SKIA_DIR}/include/pipe
		${VD_EXT_SKIA_DIR}/include/ports
		${VD_EXT_SKIA_DIR}/include/svg
		${VD_EXT_SKIA_DIR}/include/text
		${VD_EXT_SKIA_DIR}/include/utils
		${VD_EXT_SKIA_DIR}/include/views
		${VD_EXT_SKIA_DIR}/include/xml
		${VD_EXT_SKIA_DIR}/include/utils/mac
		${VD_EXT_SKIA_DIR}/include/utils/win
		${VD_EXT_SKIA_DIR}/include/utils/unix
		${VD_EXT_SKIA_DIR}/src/${VD_OS_NAME} )
		
endif()

