if(VOID_LINK_SHARED_LIBS)
set( VD_EXT_LIB_EXT					${VD_SHARED_LIB_EXT} )
else()
set( VD_EXT_LIB_EXT					${VD_STATIC_LIB_EXT} )
endif()

set( VD_EXT_DIR						${VD_ROOT_DIR}/external )
set( VD_EXT_SRC_DIR                 ${VD_EXT_DIR}/source )
set( VD_EXT_SYS_DIR                 ${VD_EXT_DIR}/${VD_SYSTEM_PLATFORM_NAME} )

set( VD_EXT_SIMD_DIR				${VD_EXT_SYS_DIR}/simd/latest )
include_directories(				${VD_EXT_SIMD_DIR}/include )

set( VD_EXT_GTEST_DIR				${VD_EXT_SYS_DIR}/gtest/latest )
include_directories(				${VD_EXT_GTEST_DIR}/include )

if(VOID_BUILD_TESTS)
	file( GLOB VD_EXT_GTEST_LIBS	${VD_EXT_GTEST_DIR}/lib/*.${VD_EXT_LIB_EXT} )
endif()

execute_process(COMMAND 			module load zlib/latest)
include_directories(				$ENV{BLDR_ZLIB_INCLUDE_PATH} )
file( GLOB VD_EXT_ZLIB_LIBS			$ENV{BLDR_ZLIB_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

execute_process(COMMAND 			module load bzip2/latest)
include_directories(				$ENV{BLDR_BZIP2_INCLUDE_PATH} )
file( GLOB VD_EXT_BZIP2_LIBS		$ENV{BLDR_BZIP2_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

execute_process(COMMAND 			module load libiconv/latest)
set( VD_EXT_ICONV_DIR				$ENV{BLDR_LIBICONV_BASE_PATH} )
include_directories(				$ENV{BLDR_LIBICONV_INCLUDE_PATH} )

execute_process(COMMAND 			module load libicu/latest)
include_directories(				$ENV{BLDR_ICU_INCLUDE_PATH} )
set( VD_EXT_ICU_DIR					$ENV{BLDR_ICU_BASE_PATH} )

execute_process(COMMAND 			module load boost/latest)
set( VD_EXT_BOOST_DIR				$ENV{BLDR_BOOST_BASE_PATH} )
include_directories(				$ENV{BLDR_BOOST_INCLUDE_PATH} )
include_directories(				$ENV{BLDR_BOOST_INCLUDE_PATH}/boost )
file( GLOB VD_EXT_BOOST_LIBS		$ENV{BLDR_BOOST_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

if(VOID_USE_NANOFLANN)
	execute_process(COMMAND 		module load nanoflann/latest)
	set( VD_EXT_NANOFLANN_DIR		$ENV{BLDR_NANOFLANN_BASE_PATH} )
	include_directories(			$ENV{BLDR_NANOFLANN_INCLUDE_PATH} )
endif()

if(VOID_USE_CURL)
	set( VD_EXT_CURL_DIR			${VD_EXT_SYS_DIR}/curl/latest )
	file( GLOB VD_EXT_CURL_LIBS		${VD_EXT_CURL_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_CURL_DIR}/include )
endif()

if(VOID_USE_HALF)
	set( VD_EXT_HALF_DIR			${VD_EXT_SRC_DIR}/half/latest )
	file( GLOB VD_EXT_HALF_SRC		${VD_EXT_HALF_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_HALF_DIR}/include)
endif()

if(VOID_USE_ASMLIB)
	set( VD_EXT_ALIB_DIR			${VD_EXT_SYS_DIR}/alib/latest )
	file( GLOB VD_EXT_ALIB_LIBS		${VD_EXT_ALIB_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_ALIB_DIR}/include)
endif()

if(VOID_USE_OPENGL)

	execute_process(COMMAND 		module load fontconfig/latest)
	set( VD_EXT_FC_DIR				$ENV{BLDR_FONTCONFIG_BASE_PATH} )
	file( GLOB VD_EXT_FC_LIBS 	 	$ENV{BLDR_FONTCONFIG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(			$ENV{BLDR_FONTCONFIG_INCLUDE_PATH} )

	execute_process(COMMAND 		module load freetype/latest)
	set( VD_EXT_FT_DIR				$ENV{BLDR_FREETYPE_BASE_PATH} )
	file( GLOB VD_EXT_FT_LIBS 	 	$ENV{BLDR_FREETYPE_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(			$ENV{BLDR_FREETYPE_INCLUDE_PATH} )

	execute_process(COMMAND 		module load freetype-gl/latest)
	set( VD_EXT_FTGL_DIR			$ENV{BLDR_FREETYPE_GL_BASE_PATH} )
	file( GLOB VD_EXT_FTGL_LIBS	 	$ENV{BLDR_FREETYPE_GL_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(			$ENV{BLDR_FREETYPE_GL_INCLUDE_PATH} )

	execute_process(COMMAND 		module load glew/latest)
	set( VD_EXT_GLEW_DIR			$ENV{BLDR_GLEW_BASE_PATH} )
	file( GLOB VD_EXT_GLEW_LIBS		${VD_EXT_GLEW_DIR}/lib/*.${VD_EXT_LIB_EXT} )
	include_directories(			$ENV{BLDR_GLEW_INCLUDE_PATH} )

	find_package(OpenGL 			REQUIRED)
	find_library(OPENGL_LIBRARY 	OpenGL)

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

if(VOID_USE_GWEN)
	set( VD_EXT_GWEN_DIR			${VD_EXT_SYS_DIR}/gwen/latest )
	file( GLOB VD_EXT_GWEN_LIBS 	${VD_EXT_GWEN_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(			${VD_EXT_GWEN_DIR}/include )
endif()

if(VOID_USE_JSON)
	set( VD_EXT_JSON_DIR			${VD_EXT_SYS_DIR}/json/latest )
	file( GLOB VD_EXT_JSON_SRC		${VD_EXT_JSON_DIR}/src/*.cpp )
	include_directories(			${VD_EXT_JSON_DIR}/src  ${VD_EXT_JSON_DIR}/include )
	set( VD_EXT_SRC 				${VD_EXT_JSON_SRC} ${VD_EXT_SRC} )
endif()

if(VOID_USE_MPI)
	execute_process(COMMAND 		module load openmpi/latest)
	set( VD_EXT_MPI_DIR				$ENV{BLDR_OPENMPI_BASE_PATH} )
	file( GLOB VD_EXT_MPI_LIBS		$ENV{BLDR_OPENMPI_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		$ENV{BLDR_OPENMPI_INCLUDE_PATH} )
endif()
	
if(VOID_USE_MSGPACK)
	execute_process(COMMAND 		module load msgpack/latest)
	set( VD_EXT_MSGPACK_DIR			$ENV{BLDR_MSGPACK_BASE_PATH} )
	file( GLOB VD_EXT_MSGPACK_LIBS	$ENV{BLDR_MSGPACK_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		$ENV{BLDR_MSGPACK_INCLUDE_PATH} )
endif()

if(VOID_USE_HDF5)
	execute_process(COMMAND 		module load szip/latest)
	set( VD_EXT_SZIP_DIR			$ENV{BLDR_SZIP_BASE_PATH} )
	file( GLOB VD_EXT_SZIP_LIBS		$ENV{BLDR_SZIP_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		$ENV{BLDR_SZIP_INCLUDE_PATH} )

	execute_process(COMMAND 		module load hdf5/latest)
	set( VD_EXT_HDF5_DIR			$ENV{BLDR_HDF5_BASE_PATH} )
	file( GLOB VD_EXT_HDF5_LIBS		$ENV{BLDR_HDF5_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    		$ENV{BLDR_HDF5_INCLUDE_PATH} )
	include_directories(    		$ENV{BLDR_HDF5_INCLUDE_PATH}/cpp )

	if(VOID_USE_F3D)
		execute_process(COMMAND 	module load field3d/latest)
		set( VD_EXT_F3D_DIR			$ENV{BLDR_FIELD3D_BASE_PATH} )
		file( GLOB VD_EXT_F3D_LIBS 	$ENV{BLDR_FIELD3D_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
		include_directories(		$ENV{BLDR_FIELD3D_INCLUDE_PATH} )
		include_directories(		$ENV{BLDR_FIELD3D_INCLUDE_PATH}/Field3D )
	endif()
endif()

if(VOID_USE_OIIO)

	execute_process(COMMAND 		module load libpng/latest)
	set( VD_EXT_PNG_DIR				$ENV{BLDR_LIBPNG_BASE_PATH} )
	file( GLOB VD_EXT_PNG_LIBS		$ENV{BLDR_LIBPNG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

	execute_process(COMMAND 		module load libjpeg/latest)
	set( VD_EXT_JPEG_DIR			$ENV{BLDR_LIBJPEG_BASE_PATH} )
	file( GLOB VD_EXT_JPEG_LIBS	 	$ENV{BLDR_LIBJPEG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

	execute_process(COMMAND 		module load libtiff/latest)
	set( VD_EXT_TIFF_DIR			$ENV{BLDR_LIBTIFF_BASE_PATH} )
	file( GLOB VD_EXT_TIFF_LIBS	 	$ENV{BLDR_LIBTIFF_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

	execute_process(COMMAND 		module load openjpeg/latest)
	set( VD_EXT_JP2K_DIR			$ENV{BLDR_OPENJPEG_BASE_PATH} )
	file( GLOB VD_EXT_JP2K_LIBS	 	$ENV{BLDR_OPENJPEG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

	execute_process(COMMAND 		module load openexr/latest)
	set( VD_EXT_EXR_DIR				$ENV{BLDR_OPENEXR_BASE_PATH} )
	file( GLOB VD_EXT_EXR_LIBS 	 	$ENV{BLDR_OPENEXR_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

	execute_process(COMMAND 		module load oiio/latest)
	set( VD_EXT_OIIO_DIR			$ENV{BLDR_OIIO_BASE_PATH} )
	file( GLOB VD_EXT_OIIO_LIBS		$ENV{BLDR_OIIO_LIB_PATH}/*.${VD_SHARED_LIB_EXT} )

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
	file( GLOB VD_EXT_SFML_LIBS 	${VD_EXT_SFML_DIR}/lib/*-s.${VD_EXT_LIB_EXT} )
	include_directories(			${VD_EXT_SFML_DIR}/include ${VD_EXT_SFML_DIR}/include/SFML )
endif()

set( VD_EXT_SRC
	${VD_EXT_HALF_SRC}
	${VD_EXT_FTGL_SRC} 
)

set( VD_EXT_LIB 				    
	${VD_EXT_BOOST_LIBS} 
	${VD_EXT_ICONV_LIBS} 
	${VD_EXT_ICU_LIBS} 
	${VD_EXT_ZLIB_LIBS}
	${VD_EXT_ALIB_LIBS}
	${VD_EXT_FC_LIBS} 
	${VD_EXT_FT_LIBS} 
	${VD_EXT_FTGL_LIBS}
	${VD_EXT_GLEW_LIBS} 
	${VD_EXT_MPI_LIBS}
	${VD_EXT_GWEN_LIBS}
	${VD_EXT_MSGPACK_LIBS}
	${VD_EXT_ICONV_LIBS}
	${VD_EXT_HDF5_LIBS}
	${VD_EXT_SZIP_LIBS}
	${VD_EXT_SFML_LIBS}
	${VD_EXT_EXR_LIBS}
	${VD_EXT_PNG_LIBS}
	${VD_EXT_JPEG_LIBS}
	${VD_EXT_JP2K_LIBS}
	${VD_EXT_F3D_LIBS}
	${VD_EXT_TIFF_LIBS}
	${VD_EXT_OIIO_LIBS}
	${OPENGL_LIBS} 	
	${GLUT_LIBS}
)
