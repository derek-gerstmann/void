# setup version numbers
set( VOID_PROJECT_NAME 			"VoidFramework")
set( VOID_VERSION_MAJOR 		1)
set( VOID_VERSION_MINOR 		0)
set( VOID_VERSION_PATCH 		0)
set( VOID_VERSION_SUFFIX 		"${CMAKE_BUILD_TYPE}")
set( VOID_VERSION_NAMESPACE 	"v${VOID_VERSION_MAJOR}p${VOID_VERSION_MINOR}")
set( VOID_SHORT_VERSION_STRING 	"${VOID_VERSION_MAJOR}.${VOID_VERSION_MINOR}")
set( VOID_VERSION_STRING 		"${VOID_VERSION_MAJOR}.${VOID_VERSION_MINOR}.${VOID_VERSION_PATCH}-${VOID_VERSION_SUFFIX}")

set( VD_FRAMEWORK_DIR  			${VD_SRC_DIR}/framework)
set( VD_CORE_DIR  				${VD_SRC_DIR}/core)
set( VD_CONSTANTS_DIR  			${VD_SRC_DIR}/constants)
set( VD_CONTAINERS_DIR  		${VD_SRC_DIR}/containers)
set( VD_FORMATS_DIR  			${VD_SRC_DIR}/formats)
set( VD_GRAPHICS_DIR  			${VD_SRC_DIR}/graphics)
set( VD_RENDER_DIR  			${VD_SRC_DIR}/render)
set( VD_INTERFACE_DIR			${VD_SRC_DIR}/interface)
set( VD_RUNTIME_DIR  			${VD_SRC_DIR}/runtime)
set( VD_STORAGE_DIR  			${VD_SRC_DIR}/storage)
set( VD_CONSTANTS_DIR  			${VD_SRC_DIR}/constants)
set( VD_EXTRACTED_DIR 			${VD_SRC_DIR}/constants/extracted)
set( VD_SYMBOLS_H 				"${VD_EXTRACTED_DIR}/symbols.h" )
set( VD_RESOURCES_TXT 			"${VD_EXTRACTED_DIR}/resources.txt" )

file( GLOB VD_FRAMEWORK_INC  	${VD_FRAMEWORK_DIR}/*.h ${VD_FRAMEWORK_DIR}/*.hpp)
file( GLOB VD_FRAMEWORK_SRC  	${VD_FRAMEWORK_DIR}/*.c ${VD_FRAMEWORK_DIR}/*.cpp)
file( GLOB VD_CORE_INC  		${VD_CORE_DIR}/*.h ${VD_CORE_DIR}/*.hpp)
file( GLOB VD_CORE_SRC  		${VD_CORE_DIR}/*.c ${VD_CORE_DIR}/*.cpp)
file( GLOB VD_CONSTANTS_INC  	${VD_CONSTANTS_DIR}/*.h ${VD_CONSTANTS_DIR}/*.hpp)
file( GLOB VD_CONSTANTS_SRC  	${VD_CONSTANTS_DIR}/*.c ${VD_CONSTANTS_DIR}/*.cpp)
file( GLOB VD_CONTAINERS_INC  	${VD_CONTAINERS_DIR}/*.h ${VD_CONTAINERS_DIR}/*.hpp)
file( GLOB VD_CONTAINERS_SRC  	${VD_CONTAINERS_DIR}/*.c ${VD_CONTAINERS_DIR}/*.cpp)
file( GLOB VD_FORMATS_INC  		${VD_FORMATS_DIR}/*.h ${VD_FORMATS_DIR}/*.hpp)
file( GLOB VD_FORMATS_SRC  		${VD_FORMATS_DIR}/*.c ${VD_FORMATS_DIR}/*.cpp)
file( GLOB VD_GRAPHICS_INC  	${VD_GRAPHICS_DIR}/*.h ${VD_GRAPHICS_DIR}/*.hpp ${VD_GRAPHICS_DIR}/*/*.hpp)
file( GLOB VD_GRAPHICS_SRC  	${VD_GRAPHICS_DIR}/*.c ${VD_GRAPHICS_DIR}/*.cpp ${VD_GRAPHICS_DIR}/*/*.cpp)
file( GLOB VD_RENDER_INC  		${VD_RENDER_DIR}/*.h ${VD_RENDER_DIR}/*.hpp)
file( GLOB VD_RENDER_SRC  		${VD_RENDER_DIR}/*.c ${VD_RENDER_DIR}/*.cpp)
file( GLOB VD_COMPUTE_INC  		${VD_COMPUTE_DIR}/*.h ${VD_COMPUTE_DIR}/*.hpp)
file( GLOB VD_COMPUTE_SRC  		${VD_COMPUTE_DIR}/*.c ${VD_COMPUTE_DIR}/*.cpp)
file( GLOB VD_INTERFACE_INC  	${VD_INTERFACE_DIR}/*.h ${VD_INTERFACE_DIR}/*.hpp ${VD_INTERFACE_DIR}/*/*.h)
file( GLOB VD_INTERFACE_SRC  	${VD_INTERFACE_DIR}/*.c ${VD_INTERFACE_DIR}/*.cpp ${VD_INTERFACE_DIR}/*/*.cpp)
file( GLOB VD_RUNTIME_INC  		${VD_RUNTIME_DIR}/*.h ${VD_RUNTIME_DIR}/*.hpp ${VD_RUNTIME_DIR}/*/*.h)
file( GLOB VD_RUNTIME_SRC  		${VD_RUNTIME_DIR}/*.c ${VD_RUNTIME_DIR}/*.cpp ${VD_RUNTIME_DIR}/*/*.cpp)
file( GLOB VD_STORAGE_INC  		${VD_STORAGE_DIR}/*.h ${VD_STORAGE_DIR}/*.hpp )
file( GLOB VD_STORAGE_SRC  		${VD_STORAGE_DIR}/*.c ${VD_STORAGE_DIR}/*.cpp )
file( GLOB VD_GLUT_RUNTIME_INC  ${VD_INTERFACE_INC}/glut/*.h)
file( GLOB VD_GLUT_RUNTIME_SRC  ${VD_INTERFACE_SRC}/glut/*.cpp)
file( GLOB VD_GWEN_INTERFACE_INC ${VD_INTERFACE_DIR}/gwen/*.h)
file( GLOB VD_GWEN_INTERFACE_SRC ${VD_INTERFACE_DIR}/gwen/*.cpp)

set( VD_ALL_SRC 
	${VD_FRAMEWORK_SRC} 
	${VD_CORE_SRC} 
	${VD_CONSTANTS_SRC} 
	${VD_CONTAINERS_SRC} 
	${VD_FORMATS_SRC} 
	${VD_GRAPHICS_SRC} 
	${VD_RENDER_SRC} 
	${VD_COMPUTE_SRC} 
	${VD_INTERFACE_SRC} 
	${VD_RUNTIME_SRC}
	${VD_STORAGE_SRC}
)

set( VD_ALL_INC
	${VD_FRAMEWORK_INC} 
	${VD_CORE_INC} 
	${VD_CONSTANTS_INC} 
	${VD_CONTAINERS_INC} 
	${VD_FORMATS_INC} 
	${VD_GRAPHICS_INC} 
	${VD_RENDER_INC} 
	${VD_COMPUTE_INC} 
	${VD_INTERFACE_INC} 
	${VD_RUNTIME_INC}
	${VD_STORAGE_INC}
)

add_definitions(${VD_DEFINITIONS})
add_definitions( -DVD_VERSION_MAJOR=${VOID_VERSION_MAJOR} )
add_definitions( -DVD_VERSION_MINOR=${VOID_VERSION_MINOR} )
add_definitions( -DVD_VERSION_PATCH=${VOID_VERSION_PATCH} )
add_definitions( -DVD_VERSION_NAMESPACE=${VOID_VERSION_NAMESPACE} )
add_definitions( -DVD_VERSION_SUFFIX=${VOID_VERSION_SUFFIX} )

if(VOID_BUILD_SHARED_LIBS)
  set( VD_LIBRARY_TYPE SHARED )
  add_definitions( -DVD_BUILD_SHARED_LIBRARY=VD_ON )
else()
  set( VD_LIBRARY_TYPE STATIC )
endif()

if(VOID_USE_DOUBLE_PRECISION)
	add_definitions( -DVD_USE_DOUBLE_PRECISION=VD_ON )
else()
	add_definitions( -DVD_USE_SINGLE_PRECISION=VD_ON )
endif()

if(VOID_USE_ASMLIB)
	add_definitions( -DVD_USE_ASMLIB=VD_ON )
endif()

if(VOID_USE_BOOST_CONTAINERS)
	add_definitions( -DVD_USE_BOOST_CONTAINERS=VD_ON )
endif()

if(VOID_USE_OPENGL)
	add_definitions( -DVD_USE_OPENGL=VD_ON )
	add_definitions( -DGLEW_STATIC=1 )
endif()

if(VOID_USE_GLUT)
	add_definitions( -DVD_USE_GLUT_RUNTIME=VD_ON )
	add_definitions( -DVD_USE_GLUT=VD_ON )
endif()

if(VOID_USE_GWEN)
	add_definitions( -DVD_USE_GWEN=VD_ON )
endif()

if(VOID_USE_SKIA)
	add_definitions( -DVD_USE_SKIA=VD_ON )
endif()

if(VOID_USE_GS2)
	add_definitions( -DVD_USE_GS2=VD_ON )
endif()

if(VOID_USE_HDF5)
	add_definitions( -DVD_USE_HDF5=VD_ON )
	if(VOID_USE_FLASH)
		add_definitions( -DVD_USE_FLASH=VD_ON )
	endif()
endif()

if(VOID_USE_MSGPACK)
	add_definitions( -DVD_USE_MSGPACK=VD_ON )
endif()

if(VOID_USE_JSON)
	add_definitions( -DVD_USE_JSON=VD_ON )
endif()

if(VOID_USE_EXTRACTED_SYMBOLS)
	set( VD_SYMEX_FILELIST ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} ${VD_PLATFORM_SRC} )
	set( VD_ALL_SRC ${VD_ALL_SRC} ${VD_SYMBOLS_H} )
	add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=VD_ON )
	add_definitions( -DVD_EXTRACTED_SYMBOLS_H="${VD_SYMBOLS_H}" )
	add_custom_command ( 
		OUTPUT ${VD_SYMBOLS_H}
		COMMENT "Extracting Static Symbols and Strings to ${VD_SYMBOLS_H} ..."
		COMMAND mkdir -p ${VD_EXTRACTED_DIR}
		COMMAND VdSymEx ${VD_SYMEX_FILELIST} -i "core/core.h" -i "core/symbol.h" -i "constants/constants.h" -n extracted -n Symbols -o ${VD_SYMBOLS_H}
		DEPENDS ${VD_ALL_SRC} ${VD_ALL_INC} ${VD_ALL_OPT_SRC} ${VD_ALL_OPT_INC} ${VD_PLATFORM_SRC} ${VD_TOOLS_SYMEX_SRC}
	)
endif()

add_definitions( ${VD_DEFINITIONS} )

include_directories(
    ${VD_SRC_DIR}
	${VD_FRAMEWORK_DIR}
	${VD_CORE_DIR}
	${VD_CONSTANTS_DIR}
	${VD_CONTAINERS_DIR}
	${VD_FORMATS_DIR}
	${VD_GRAPHICS_DIR}
	${VD_RENDER_DIR}
	${VD_COMPUTE_DIR} 
	${VD_INTERFACE_DIR}
	${VD_RUNTIME_DIR}
	${VD_STORAGE_DIR}
	${VD_CONSTANTS_DIR}
)

if(VOID_USE_GLUT)
	find_package(GLUT) 
	find_package(OpenGL)
	if(GLUT_FOUND)
		include_directories(${GLUT_INCLUDE_DIR})
	endif(GLUT_FOUND)
	if(OPENGL_FOUND)
		include_directories(${OPENGL_INCLUDE_DIR})
	endif(OPENGL_FOUND)
	add_library( VdGlutRuntime ${VD_LIBRARY_TYPE} ${VD_GLUT_RUNTIME_SRC} ${VD_GLUT_RUNTIME_INC} )
endif()

if(VOID_USE_GWEN)
	add_definitions( -DVD_USE_GWEN=VD_ON )
	add_library( VdGwenInterface ${VD_LIBRARY_TYPE} ${VD_GWEN_INTERFACE_SRC} ${VD_GWEN_INTERFACE_INC} )
endif()

add_library( VdFramework ${VD_LIBRARY_TYPE} ${VD_ALL_SRC} ${VD_ALL_INC} )

message(STATUS "Configured: Void Framework ${VOID_VERSION_STRING} ${CMAKE_BUILD_TYPE}")
