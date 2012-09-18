set( VD_EXT_DIR					${VD_ROOT_DIR}/external )
set( VD_EXT_SRC_DIR                 		${VD_EXT_DIR}/source )
set( VD_EXT_SYS_DIR                 		${VD_EXT_DIR}/${VD_SYSTEM_PLATFORM_NAME} )
set( VD_EXT_SIMD_DIR				${VD_EXT_SYS_DIR}/simd/latest )
set( VD_EXT_ICONV_DIR				$ENV{BLDR_LIBICONV_BASE_PATH} )
set( VD_EXT_ICU_DIR				$ENV{BLDR_ICU_BASE_PATH} )
set( VD_EXT_FLANN_DIR				$ENV{BLDR_FLANN_BASE_PATH} )
set( VD_EXT_ZLIB_DIR				$ENV{BLDR_ZLIB_BASE_PATH} )
set( VD_EXT_BOOST_DIR				$ENV{BLDR_BOOST_BASE_PATH} )

if(VOID_LINK_SHARED_LIBS)
set( VD_EXT_LIB_EXT				${VD_SHARED_LIB_EXT} )
else()
set( VD_EXT_LIB_EXT				${VD_STATIC_LIB_EXT} )
endif()

include_directories(				$ENV{VD_EXT_SIMD_DIR}/include )
include_directories(				$ENV{BLDR_ICONV_INCLUDE_PATH} )
include_directories(				$ENV{BLDR_ICU_INCLUDE_PATH} )
include_directories(				$ENV{BLDR_BOOST_INCLUDE_PATH} )
include_directories(				$ENV{BLDR_BOOST_INCLUDE_PATH}/boost )
include_directories(				$ENV{BLDR_ZLIB_INCLUDE_PATH} )
include_directories(				$ENV{BLDR_FLANN_INCLUDE_PATH} )

file( GLOB VD_EXT_BOOST_LIB			${VD_EXT_BOOST_DIR}/lib/*.${VD_EXT_LIB_EXT} )
file( GLOB VD_EXT_ZLIB_LIB			${VD_EXT_ZLIB_DIR}/lib/*.${VD_EXT_LIB_EXT} )

if(VOID_USE_CURL)
	set( VD_EXT_CURL_DIR			${VD_EXT_SYS_DIR}/curl/latest )
	file( GLOB VD_EXT_CURL_LIB		${VD_EXT_CURL_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_CURL_DIR}/include )
endif()

if(VOID_USE_HALF)
	set( VD_EXT_HALF_DIR			${VD_EXT_SRC_DIR}/half/latest )
	file( GLOB VD_EXT_HALF_SRC		${VD_EXT_HALF_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_HALF_DIR}/include)
endif()

if(VOID_USE_ASMLIB)
	set( VD_EXT_ALIB_DIR			${VD_EXT_SYS_DIR}/alib/latest )
	file( GLOB VD_EXT_ALIB_LIB		${VD_EXT_ALIB_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_ALIB_DIR}/include)
endif()

if(VOID_USE_OPENGL)
	
	set( VD_EXT_FC_DIR			$ENV{BLDR_FONTCONFIG_BASE_PATH} )
	set( VD_EXT_FT_DIR			$ENV{BLDR_FREETYPE_BASE_PATH} )
	set( VD_EXT_FTGL_DIR			$ENV{BLDR_FREETYPE_GL_BASE_PATH} )
	set( VD_EXT_GLEW_DIR			$ENV{BLDR_GLEW_BASE_PATH} )

	file( GLOB VD_EXT_ICONV_LIB		${VD_EXT_ICONV_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_ICU_LIB		${VD_EXT_ICU_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_FC_LIB 	 	${VD_EXT_FC_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_FT_LIB 	 	${VD_EXT_FT_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_GLEW_LIB		${VD_EXT_GLEW_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_FTGL_SRC 	 	${VD_EXT_FTGL_DIR}/src/*.c )

	find_package(OpenGL 			REQUIRED)
	find_library(OPENGL_LIBRARY 		OpenGL)

	if(VD_SYSTEM_OSX)
	    find_path(OPENGL_INCLUDE_DIR "OpenGL/gl.h" DOC "Include for OpenGL on OSX")
		set (OPENGL_gl_LIBRARY "-framework OpenGL" CACHE STRING "OpenGL lib for OSX")
		set (OPENGL_glu_LIBRARY "-framework AGL" CACHE STRING "AGL lib for OSX")	
		set (OPENGL_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})	
	    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
	    	/System/Library/Frameworks/OpenGL.framework/Headers ${VD_EXT_SYS_DIR}/opengl/latest/include/OpenGL)
	else()
	    find_path(OPENGL_INCLUDE_DIR GL/gl.h DOC "Include for OpenGL")
		set (OPENGL_LIBS ${OPENGL_LIBRARY})		
	endif()

	include_directories(${VD_EXT_SYS_DIR}/opengl/latest/include)

	include_directories(
		${VD_EXT_FC_DIR}/include
		${VD_EXT_FT_DIR}/include
		${VD_EXT_FT_DIR}/include/freetype2
		${VD_EXT_FTGL_DIR}/include
		${VD_EXT_GLEW_DIR}/src
		${VD_EXT_GLEW_DIR}/include
		${VD_EXT_GLEW_DIR}/include/GL)

	if(VOID_USE_GLUT)
		if(VD_SYSTEM_IS_LINUX)
			set( VD_EXT_GLUT_DIR 	$ENV{BLDR_FREEGLUT_BASE_PATH} )
			set( GLUT_INCLUDE_DIRS	${VD_EXT_GLUT_DIR}/include ${VD_EXT_GLUT_DIR}/include/GL )
			file( GLOB GLUT_LIBRARY ${VD_EXT_GLUT_DIR}/lib/*.${VD_EXT_LIB_EXT} )
		endif()
		if(VD_SYSTEM_IS_OSX)
			find_package(GLUT 		REQUIRED)
			find_library(GLUT_LIBRARY Glut )
			find_path(GLUT_INCLUDE_DIR Glut/glut.h)
			set (GLUT_glut_LIBRARY "-framework Glut" CACHE STRING "Glut lib for OSX")
			set (GLUT_LIBS ${GLUT_glut_LIBRARY})
		    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
		    	/System/Library/Frameworks/Glut.framework/Headers ${VD_EXT_SYS_DIR}/opengl/latest/include/GLUT)
		else()
			find_package(GLUT 		REQUIRED)
			find_path(GLUT_INCLUDE_DIR GLUT/glut.h)
		endif()

		include_directories(${GLUT_INCLUDE_DIRS})
	endif()
		
endif()

set( VD_EXT_GTEST_DIR				$ENV{BLDR_GTEST_BASE_PATH} )
include_directories(				${VD_EXT_GTEST_DIR}/include )

if(VOID_BUILD_TESTS)
	file( GLOB VD_EXT_GTEST_LIB	 	${VD_EXT_GTEST_DIR}/lib/*.${VD_EXT_LIB_EXT} )
endif()

if(VOID_USE_GWEN)
	set( VD_EXT_GWEN_DIR			${VD_EXT_SYS_DIR}/gwen/latest )
	file( GLOB VD_EXT_GWEN_LIB 	 	${VD_EXT_GWEN_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(			${VD_EXT_GWEN_DIR}/include )
endif()

if(VOID_USE_JSON)
	set( VD_EXT_JSON_DIR			${VD_EXT_SYS_DIR}/json/latest )
	file( GLOB VD_EXT_JSON_SRC		${VD_EXT_JSON_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_JSON_DIR}/src  ${VD_EXT_JSON_DIR}/include )
	set( VD_EXT_SRC 			${VD_EXT_JSON_SRC} ${VD_EXT_SRC} )
endif()

if(VOID_USE_MPI)
	set( VD_EXT_MPI_DIR			$ENV{BLDR_OPENMPI_BASE_PATH} )
	file( GLOB VD_EXT_MPI_LIB		${VD_EXT_MPI_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		${VD_EXT_MPI_DIR}/include )
endif()
	
if(VOID_USE_MSGPACK)
	set( VD_EXT_MSGPACK_DIR			$ENV{BLDR_MSGPACK_BASE_PATH} )
	file( GLOB VD_EXT_MSGPACK_LIB		${VD_EXT_MSGPACK_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		${VD_EXT_MSGPACK_DIR}/include )
endif()

if(VOID_USE_HDF5)
	set( VD_EXT_SZIP_DIR			$ENV{BLDR_SZIP_BASE_PATH} )
	file( GLOB VD_EXT_SZIP_LIB		${VD_EXT_SZIP_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		${VD_EXT_SZIP_DIR}/include )

	set( VD_EXT_HDF5_DIR			$ENV{BLDR_HDF5_BASE_PATH} )
	file( GLOB VD_EXT_HDF5_LIB		${VD_EXT_HDF5_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		${VD_EXT_HDF5_DIR}/include )
	include_directories(    		${VD_EXT_HDF5_DIR}/include/cpp )

	if(VOID_USE_F3D)
		set( VD_EXT_F3D_DIR		$ENV{BLDR_FIELD3D_BASE_PATH} )
		file( GLOB VD_EXT_F3D_LIB 	${VD_EXT_F3D_DIR}/lib/*.${VD_EXT_LIB_EXT} )
		include_directories(		${VD_EXT_F3D_DIR}/include ${VD_EXT_F3D_DIR}/include/Field3D )
	endif()
endif()

if(VOID_USE_OIIO)
	set( VD_EXT_OIIO_DIR			$ENV{BLDR_OIIO_BASE_PATH} )
	set( VD_EXT_PNG_DIR			$ENV{BLDR_LIBPNG_BASE_PATH} )
	set( VD_EXT_JPEG_DIR			$ENV{BLDR_LIBJPEG_BASE_PATH} )
	set( VD_EXT_TIFF_DIR			$ENV{BLDR_LIBTIFF_BASE_PATH} )
	set( VD_EXT_JP2K_DIR			$ENV{BLDR_OPENJPEG_BASE_PATH} )
	set( VD_EXT_EXR_DIR			$ENV{BLDR_OPENEXR_BASE_PATH} )

	file( GLOB VD_EXT_OIIO_LIB		${VD_EXT_OIIO_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_PNG_LIB		${VD_EXT_PNG_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_EXR_LIB 	 	${VD_EXT_EXR_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_JPEG_LIB 	 	${VD_EXT_JPEG_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_JP2K_LIB 	 	${VD_EXT_JP2K_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	file( GLOB VD_EXT_TIFF_LIB 	 	${VD_EXT_TIFF_DIR}/lib/*.${VD_EXT_LIB_EXT} )

	include_directories(
		${VD_EXT_OIIO_DIR}/include
		${VD_EXT_EXR_DIR}/include
		${VD_EXT_EXR_DIR}/include/OpenEXR
		${VD_EXT_PNG_DIR}/include
		${VD_EXT_TIFF_DIR}/include
		${VD_EXT_JP2K_DIR}/include
		${VD_EXT_JPEG_DIR}/include
		${VD_EXT_JPEG_DIR}/include/jpeg )
	
endif()

if(VOID_USE_SFML)
	set( VD_EXT_SFML_DIR			${VD_EXT_SYS_DIR}/sfml/latest )
	file( GLOB VD_EXT_SFML_LIB 	 	${VD_EXT_SFML_DIR}/lib/*-s.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_SFML_DIR}/include ${VD_EXT_SFML_DIR}/include/SFML )
endif()

set( VD_EXT_SRC
	${VD_EXT_HALF_SRC}
	${VD_EXT_FTGL_SRC} 
)

set( VD_EXT_LIB 				    
	${VD_EXT_BOOST_LIB} 
	${VD_EXT_ICONV_LIB} 
	${VD_EXT_ICU_LIB} 
	${VD_EXT_ZLIB_LIB}
	${VD_EXT_ALIB_LIB}
	${VD_EXT_FC_LIB} 
	${VD_EXT_FT_LIB} 
	${VD_EXT_GLEW_LIB} 
	${VD_EXT_SKIA_LIB}
	${VD_EXT_MPI_LIB}
	${VD_EXT_GWEN_LIB}
	${VD_EXT_MSGPACK_LIB}
	${VD_EXT_ICONV_LIB}
	${VD_EXT_HDF5_LIB}
	${VD_EXT_SZIP_LIB}
	${VD_EXT_SFML_LIB}
	${VD_EXT_EXR_LIB}
	${VD_EXT_PNG_LIB}
	${VD_EXT_JPEG_LIB}
	${VD_EXT_JP2K_LIB}
	${VD_EXT_F3D_LIB}
	${VD_EXT_TIFF_LIB}
	${VD_EXT_OIIO_LIB}
	${OPENGL_LIBS} 	
	${GLUT_LIBS}
)
