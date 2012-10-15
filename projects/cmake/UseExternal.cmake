if(VOID_LINK_SHARED_LIBS)
set( VD_EXT_LIB_EXT							${VD_SHARED_LIB_EXT} )
else()
set( VD_EXT_LIB_EXT							${VD_STATIC_LIB_EXT} )
endif()

set( VD_EXT_DIR								${VD_ROOT_DIR}/external )
set( VD_EXT_SRC_DIR                 		${VD_EXT_DIR}/source )
set( VD_EXT_SYS_DIR                 		${VD_EXT_DIR}/${VD_SYSTEM_PLATFORM_NAME} )

set( VD_EXT_SIMD_DIR						${VD_EXT_SYS_DIR}/simd/latest )
include_directories(						${VD_EXT_SIMD_DIR}/include )

set( VD_EXT_GTEST_DIR						${VD_EXT_SYS_DIR}/gtest/latest )
include_directories(						${VD_EXT_GTEST_DIR}/include )

if(VOID_BUILD_TESTS)
	file( GLOB VD_EXT_GTEST_LIBS			${VD_EXT_GTEST_DIR}/lib/*.${VD_EXT_LIB_EXT} )
endif()

set( VD_EXT_ZLIB_DIR						${VD_EXT_SYS_DIR}/zlib/latest )
set( VD_EXT_ZLIB_INCLUDE_PATH				${VD_EXT_ZLIB_DIR}/include )
set( VD_EXT_ZLIB_LIB_PATH					${VD_EXT_ZLIB_DIR}/lib )
include_directories(						${VD_EXT_ZLIB_INCLUDE_PATH} )
file( GLOB VD_EXT_ZLIB_LIBS					${VD_EXT_ZLIB_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

set( VD_EXT_BZIP2_DIR						${VD_EXT_SYS_DIR}/bzip2/latest )
set( VD_EXT_BZIP2_INCLUDE_PATH				${VD_EXT_BZIP2_DIR}/include )
set( VD_EXT_BZIP2_LIB_PATH					${VD_EXT_BZIP2_DIR}/lib )
include_directories(						${VD_EXT_BZIP2_INCLUDE_PATH} )
file( GLOB VD_EXT_BZIP2_LIBS				${VD_EXT_BZIP2_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

set( VD_EXT_LIBICONV_DIR					${VD_EXT_SYS_DIR}/libiconv/latest )
set( VD_EXT_LIBICONV_INCLUDE_PATH			${VD_EXT_LIBICONV_DIR}/include )
set( VD_EXT_LIBICONV_LIB_PATH				${VD_EXT_LIBICONV_DIR}/lib )
include_directories(						${VD_EXT_LIBICONV_INCLUDE_PATH} )
file( GLOB VD_EXT_LIBICONV_LIBS				${VD_EXT_LIBICONV_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

set( VD_EXT_LIBICU_DIR						${VD_EXT_SYS_DIR}/libicu/latest )
set( VD_EXT_LIBICU_INCLUDE_PATH				${VD_EXT_LIBICU_DIR}/include )
set( VD_EXT_LIBICU_LIB_PATH					${VD_EXT_LIBICU_DIR}/lib )
include_directories(						${VD_EXT_LIBICU_INCLUDE_PATH} )
file( GLOB VD_EXT_LIBICU_LIBS				${VD_EXT_LIBICU_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

set( VD_EXT_BOOST_DIR						${VD_EXT_SYS_DIR}/boost/latest )
set( VD_EXT_BOOST_INCLUDE_PATH				${VD_EXT_BOOST_DIR}/include )
set( VD_EXT_BOOST_LIB_PATH					${VD_EXT_BOOST_DIR}/lib )
include_directories(						${VD_EXT_BOOST_INCLUDE_PATH} )
include_directories(						${VD_EXT_BOOST_INCLUDE_PATH}/boost )

set( VD_EXT_BOOST_LIBS				
	${VD_EXT_BOOST_LIB_PATH}/libboost_filesystem.${VD_EXT_LIB_EXT} 
	${VD_EXT_BOOST_LIB_PATH}/libboost_program_options.${VD_EXT_LIB_EXT} 
	${VD_EXT_BOOST_LIB_PATH}/libboost_system.${VD_EXT_LIB_EXT} 
	${VD_EXT_BOOST_LIB_PATH}/libboost_thread.${VD_EXT_LIB_EXT} 
)

if(VOID_USE_NANOFLANN)
	set( VD_EXT_NANOFLANN_DIR				${VD_EXT_SYS_DIR}/nanoflann/latest )
	set( VD_EXT_NANOFLANN_INCLUDE_PATH		${VD_EXT_NANOFLANN_DIR}/include )
	set( VD_EXT_NANOFLANN_LIB_PATH			${VD_EXT_NANOFLANN_DIR}/lib )
	include_directories(					${VD_EXT_NANOFLANN_INCLUDE_PATH} )
	file( GLOB VD_EXT_NANOFLANN_LIBS		${VD_EXT_NANOFLANN_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
endif()

if(VOID_USE_CURL)
	set( VD_EXT_CURL_DIR					${VD_EXT_SYS_DIR}/curl/latest )
	set( VD_EXT_CURL_INCLUDE_PATH			${VD_EXT_CURL_DIR}/include )
	set( VD_EXT_CURL_LIB_PATH				${VD_EXT_CURL_DIR}/lib )
	file( GLOB VD_EXT_CURL_LIBS				${VD_EXT_CURL_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_CURL_INCLUDE_PATH} )
endif()

if(VOID_USE_HALF)
	set( VD_EXT_HALF_DIR					${VD_EXT_SRC_DIR}/half/latest )
	file( GLOB VD_EXT_HALF_SRC				${VD_EXT_HALF_DIR}/src/*.cpp )
	include_directories(					${VD_EXT_HALF_DIR}/include)
endif()

if(VOID_USE_ASMLIB)
	set( VD_EXT_ASMLIB_DIR					${VD_EXT_SYS_DIR}/asmlib/latest )
	set( VD_EXT_ASMLIB_INCLUDE_PATH			${VD_EXT_ASMLIB_DIR}/include )
	set( VD_EXT_ASMLIB_LIB_PATH				${VD_EXT_ASMLIB_DIR}/lib )
	file( GLOB VD_EXT_ASMLIB_LIBS			${VD_EXT_ASMLIB_LIB_PATH/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_ASMLIB_INCLUDE_PATH} )
endif()

if(VOID_USE_OPENGL)
	find_package(OpenGL 					REQUIRED)
	find_library(OPENGL_LIBRARY 			OpenGL)

	if(VD_SYSTEM_OSX)
	    find_path(OPENGL_INCLUDE_DIR "OpenGL/gl.h" DOC "Include for OpenGL on OSX")
		set (OPENGL_gl_LIBRARY "-framework OpenGL" CACHE STRING "OpenGL lib for OSX")
		set (OPENGL_glu_LIBRARY "-framework AGL" CACHE STRING "AGL lib for OSX")	
		set (OPENGL_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})	
	    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
	    	/System/Library/Frameworks/OpenGL.framework/Headers ${VD_EXT_SYS_DIR}/opengl/latest/include/OpenGL)

	    set(VD_EXT_OPENGL_INCLUDE_DIR		${VD_EXT_SYS_DIR}/opengl/latest/include/OpenGL})
		set(VD_EXT_OPENGL_LIBS 				${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
	else()
	    find_path(OPENGL_INCLUDE_DIR GL/gl.h DOC "Include for OpenGL")
	    set(VD_EXT_OPENGL_INCLUDE_DIR		${OPENGL_INCLUDE_DIR})
		set(VD_EXT_OPENGL_LIBS 				${OPENGL_LIBRARY})		
	endif()
	include_directories(					${VD_EXT_OPENGL_INCLUDE_DIR})

	set( VD_EXT_FONTCONFIG_DIR				${VD_EXT_SYS_DIR}/fontconfig/latest )
	set( VD_EXT_FONTCONFIG_INCLUDE_PATH		${VD_EXT_FONTCONFIG_DIR}/include )
	set( VD_EXT_FONTCONFIG_LIB_PATH			${VD_EXT_FONTCONFIG_DIR}/lib )
	file( GLOB VD_EXT_FONTCONFIG_LIBS		${VD_EXT_FONTCONFIG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_FONTCONFIG_INCLUDE_PATH} )

	set( VD_EXT_FREETYPE_DIR				${VD_EXT_SYS_DIR}/freetype/latest )
	set( VD_EXT_FREETYPE_INCLUDE_PATH		${VD_EXT_FREETYPE_DIR}/include )
	set( VD_EXT_FREETYPE_LIB_PATH			${VD_EXT_FREETYPE_DIR}/lib )
	file( GLOB VD_EXT_FREETYPE_LIBS			${VD_EXT_FREETYPE_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_FREETYPE_INCLUDE_PATH}/freetype2 )

	set( VD_EXT_FREETYPE_GL_DIR				${VD_EXT_SYS_DIR}/freetype-gl/latest  )
	set( VD_EXT_FREETYPE_GL_INCLUDE_PATH	${VD_EXT_FREETYPE_GL_DIR}/include )
	set( VD_EXT_FREETYPE_GL_LIB_PATH		${VD_EXT_FREETYPE_GL_DIR}/lib )
	set( VD_EXT_FREETYPE_GL_SRC_PATH		${VD_EXT_FREETYPE_GL_DIR}/src )
	file( GLOB VD_EXT_FREETYPE_GL_SRCS		${VD_EXT_FREETYPE_GL_SRC_PATH}/*.c )
	file( GLOB VD_EXT_FREETYPE_GL_LIBS		${VD_EXT_FREETYPE_GL_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_FREETYPE_GL_INCLUDE_PATH} )

	set( VD_EXT_GLEW_DIR					${VD_EXT_SYS_DIR}/glew/latest )
	set( VD_EXT_GLEW_INCLUDE_PATH			${VD_EXT_GLEW_DIR}/include )
	set( VD_EXT_GLEW_LIB_PATH				${VD_EXT_GLEW_DIR}/lib )
	file( GLOB VD_EXT_GLEW_LIBS				${VD_EXT_GLEW_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories(					${VD_EXT_GLEW_INCLUDE_PATH} )
	include_directories(					${VD_EXT_GLEW_INCLUDE_PATH}/GL )

	if(VOID_USE_GLFW)
		set( VD_EXT_GLFW_DIR				${VD_EXT_SYS_DIR}/glfw/latest )
		set( VD_EXT_GLFW_INCLUDE_PATH		${VD_EXT_GLFW_DIR}/include )
		set( VD_EXT_GLFW_LIB_PATH			${VD_EXT_GLFW_DIR}/lib )
		file( GLOB VD_EXT_GLFW_LIBS			${VD_EXT_GLFW_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
		include_directories(				${VD_EXT_GLFW_INCLUDE_PATH} )
		include_directories(				${VD_EXT_GLFW_INCLUDE_PATH}/GL )
	endif()

	if(VOID_USE_GLUT)

		if(VD_SYSTEM_IS_OSX)
			find_package(GLUT 				REQUIRED)
			find_library(GLUT_LIBRARY 		Glut )
			find_path(GLUT_INCLUDE_DIR 		Glut/glut.h)
			set (GLUT_glut_LIBRARY 			"-framework Glut" CACHE STRING "Glut lib for OSX")
			set ( VD_EXT_GLUT_LIBS 			${GLUT_glut_LIBRARY})

		    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
		    	/System/Library/Frameworks/Glut.framework/Headers ${VD_EXT_SYS_DIR}/opengl/latest/include/GLUT)

			set( VD_EXT_GLUT_INCLUDE_PATH	${GLUT_INCLUDE_DIR} )

		else()
			set( VD_EXT_GLUT_DIR 			${VD_EXT_SYS_DIR}/freeglut/latest )
			set( VD_EXT_GLUT_INCLUDE_PATH	${VD_EXT_GLUT_DIR}/include )
			set( VD_EXT_GLUT_LIB_PATH		${VD_EXT_GLUT_DIR}/lib )
			set( VD_EXT_GLUT_INCLUDE_PATH	${VD_EXT_GLUT_INCLUDE_PATH})
			file( GLOB VD_EXT_GLUT_LIBS		${VD_EXT_GLUT_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
		endif()

		include_directories(				${VD_EXT_GLUT_INCLUDE_PATH})
		include_directories(				${VD_EXT_GLUT_INCLUDE_PATH}/GL)

	endif()
		
endif()

if(VOID_USE_GWEN)
	set( VD_EXT_GWEN_DIR					${VD_EXT_SYS_DIR}/gwen/latest )
	file( GLOB VD_EXT_GWEN_LIBS 			${VD_EXT_GWEN_DIR}/lib/*.${VD_EXT_LIB_EXT} )	
	include_directories(					${VD_EXT_GWEN_DIR}/include )
endif()

if(VOID_USE_JSON)
	set( VD_EXT_JSON_DIR					${VD_EXT_SYS_DIR}/json/latest )
	file( GLOB VD_EXT_JSON_SRC				${VD_EXT_JSON_DIR}/src/*.cpp )
	include_directories(					${VD_EXT_JSON_DIR}/src  ${VD_EXT_JSON_DIR}/include )
	set( VD_EXT_SRC 						${VD_EXT_JSON_SRC} ${VD_EXT_SRC} )
endif()

if(VOID_USE_MPI)
	set( VD_EXT_OPENMPI_DIR 				${VD_EXT_SYS_DIR}/openmpi/latest )
	set( VD_EXT_OPENMPI_INCLUDE_PATH		${VD_EXT_OPENMPI_DIR}/include )
	set( VD_EXT_OPENMPI_LIB_PATH			${VD_EXT_OPENMPI_DIR}/lib )
	file( GLOB VD_EXT_OPENMPI_LIBS			${VD_EXT_OPENMPI_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    				${VD_EXT_OPENMPI_INCLUDE_PATH} )
endif()
	
if(VOID_USE_MSGPACK)
	set( VD_EXT_MSGPACK_DIR 				${VD_EXT_SYS_DIR}/msgpack/latest )
	set( VD_EXT_MSGPACK_INCLUDE_PATH		${VD_EXT_MSGPACK_DIR}/include )
	set( VD_EXT_MSGPACK_LIB_PATH			${VD_EXT_MSGPACK_DIR}/lib )
	file( GLOB VD_EXT_MSGPACK_LIBS			${VD_EXT_MSGPACK_LIB_PATH}/*.${VD_EXT_LIB_EXT} )	
	include_directories(    				${VD_EXT_MSGPACK_INCLUDE_PATH} )
endif()

if(VOID_USE_HDF5)
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

	if(VOID_USE_FIELD3D)
		set( VD_EXT_FIELD3D_DIR				${VD_EXT_SYS_DIR}/field3d/latest )
		set( VD_EXT_FIELD3D_INCLUDE_PATH	${VD_EXT_FIELD3D_DIR}/include )
		set( VD_EXT_FIELD3D_LIB_PATH		${VD_EXT_FIELD3D_DIR}/lib )
		include_directories(				${VD_EXT_FIELD3D_INCLUDE_PATH} )
		include_directories(				${VD_EXT_FIELD3D_INCLUDE_PATH}/Field3D )
	endif()
endif()

if(VOID_USE_OIIO)

	set( VD_EXT_LIBPNG_DIR					${VD_EXT_SYS_DIR}/libpng/latest )
	set( VD_EXT_LIBPNG_INCLUDE_PATH			${VD_EXT_LIBPNG_DIR}/include )
	set( VD_EXT_LIBPNG_LIB_PATH				${VD_EXT_LIBPNG_DIR}/lib )
	file( GLOB VD_EXT_LIBPNG_LIBS			${VD_EXT_LIBPNG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_LIBPNG_INCLUDE_PATH} )

	set( VD_EXT_LIBJPEG_DIR					${VD_EXT_SYS_DIR}/libjpeg/latest )
	set( VD_EXT_LIBJPEG_INCLUDE_PATH		${VD_EXT_LIBJPEG_DIR}/include )
	set( VD_EXT_LIBJPEG_LIB_PATH			${VD_EXT_LIBJPEG_DIR}/lib )
	file( GLOB VD_EXT_LIBJPEG_LIBS			${VD_EXT_LIBJPEG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_LIBJPEG_INCLUDE_PATH} )
	include_directories( 					${VD_EXT_LIBJPEG_INCLUDE_PATH}/jpeg )

	set( VD_EXT_LIBTIFF_DIR					${VD_EXT_SYS_DIR}/libtiff/latest )
	set( VD_EXT_LIBTIFF_INCLUDE_PATH		${VD_EXT_LIBTIFF_DIR}/include )
	set( VD_EXT_LIBTIFF_LIB_PATH			${VD_EXT_LIBTIFF_DIR}/lib )
	file( GLOB VD_EXT_LIBTIFF_LIBS			${VD_EXT_LIBTIFF_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_LIBTIFF_INCLUDE_PATH} )

	set( VD_EXT_OPENJPEG_DIR				${VD_EXT_SYS_DIR}/openjpeg/latest )
	set( VD_EXT_OPENJPEG_INCLUDE_PATH		${VD_EXT_OPENJPEG_DIR}/include )
	set( VD_EXT_OPENJPEG_LIB_PATH			${VD_EXT_OPENJPEG_DIR}/lib )
	file( GLOB VD_EXT_OPENJPEG_LIBS			${VD_EXT_OPENJPEG_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_OPENJPEG_INCLUDE_PATH} )

	set( VD_EXT_ILMBASE_DIR					${VD_EXT_SYS_DIR}/ilmbase/latest )
	set( VD_EXT_ILMBASE_INCLUDE_PATH		${VD_EXT_ILMBASE_DIR}/include )
	set( VD_EXT_ILMBASE_LIB_PATH			${VD_EXT_ILMBASE_DIR}/lib )
	file( GLOB VD_EXT_ILMBASE_LIBS			${VD_EXT_ILMBASE_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_ILMBASE_INCLUDE_PATH} )
	include_directories( 					${VD_EXT_ILMBASE_INCLUDE_PATH}/OpenEXR )

	set( VD_EXT_OPENEXR_DIR					${VD_EXT_SYS_DIR}/openexr/latest )
	set( VD_EXT_OPENEXR_INCLUDE_PATH		${VD_EXT_OPENEXR_DIR}/include )
	set( VD_EXT_OPENEXR_LIB_PATH			${VD_EXT_OPENEXR_DIR}/lib )
	file( GLOB VD_EXT_OPENEXR_LIBS			${VD_EXT_OPENEXR_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_OPENEXR_INCLUDE_PATH} )
	include_directories( 					${VD_EXT_OPENEXR_INCLUDE_PATH}/OpenEXR )

	set( VD_EXT_OIIO_DIR					${VD_EXT_SYS_DIR}/oiio/latest )
	set( VD_EXT_OIIO_INCLUDE_PATH			${VD_EXT_OIIO_DIR}/include )
	set( VD_EXT_OIIO_LIB_PATH				${VD_EXT_OIIO_DIR}/lib )
	file( GLOB VD_EXT_OIIO_LIBS				${VD_EXT_OIIO_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_OIIO_INCLUDE_PATH} )
	include_directories( 					${VD_EXT_OIIO_INCLUDE_PATH}/OpenImageIO )	
endif()

if(VOID_USE_SFML)
	set( VD_EXT_SFML_DIR					${VD_EXT_SYS_DIR}/sfml/latest )
	set( VD_EXT_SFML_INCLUDE_PATH			${VD_EXT_SFML_DIR}/include )
	set( VD_EXT_SFML_LIB_PATH				${VD_EXT_SFML_DIR}/lib )
	file( GLOB VD_EXT_SFML_LIBS				${VD_EXT_SFML_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
	include_directories( 					${VD_EXT_SFML_INCLUDE_PATH} )
endif()

set( VD_EXT_SRC
	${VD_EXT_HALF_SRC}
	${VD_EXT_FREETYPE_GL_SRCS} 
)

set( VD_EXT_LIB 				    
	${VD_EXT_BZIP2_LIBS}
	${VD_EXT_ZLIB_LIBS}
	${VD_EXT_ASMLIB_LIBS}
	${VD_EXT_BOOST_LIBS} 
	${VD_EXT_LIBICONV_LIBS} 
	${VD_EXT_LIBICU_LIBS} 
	${VD_EXT_FONTCONFIG_LIBS} 
	${VD_EXT_FREETYPE_LIBS} 
	${VD_EXT_FREETYPE_GL_LIBS}
	${VD_EXT_GLEW_LIBS} 
	${VD_EXT_GLFW_LIBS} 
	${VD_EXT_OPENMPI_LIBS}
	${VD_EXT_GWEN_LIBS}
	${VD_EXT_MSGPACK_LIBS}
	${VD_EXT_HDF5_LIBS}
	${VD_EXT_SZIP_LIBS}
	${VD_EXT_SFML_LIBS}
	${VD_EXT_LIBPNG_LIBS}
	${VD_EXT_LIBJPEG_LIBS}
	${VD_EXT_LIBTIFF_LIBS}
	${VD_EXT_FIELD3D_LIBS}
	${VD_EXT_OPENJPEG_LIBS}
	${VD_EXT_OPENEXR_LIBS}
	${VD_EXT_ILMBASE_LIBS}
	${VD_EXT_OIIO_LIBS}
	${OPENGL_LIBS} 	
	${GLUT_LIBS}
)
