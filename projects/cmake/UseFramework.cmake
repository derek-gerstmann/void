# setup version numbers
remove_definitions( -DVD_BUILDING_FRAMEWORK )

set( VOID_PROJECT_NAME          "Void")
set( VOID_VERSION_MAJOR         1)
set( VOID_VERSION_MINOR         0)
set( VOID_VERSION_PATCH         0)
set( VOID_VERSION_SUFFIX        "${CMAKE_BUILD_TYPE}")
set( VOID_VERSION_NAMESPACE     "v${VOID_VERSION_MAJOR}p${VOID_VERSION_MINOR}")
set( VOID_SHORT_VERSION_STRING  "${VOID_VERSION_MAJOR}.${VOID_VERSION_MINOR}")
set( VOID_VERSION_STRING        "${VOID_VERSION_MAJOR}.${VOID_VERSION_MINOR}.${VOID_VERSION_PATCH}-${VOID_VERSION_SUFFIX}")

set( VD_FRAMEWORK_DIR           ${VD_SRC_DIR}/common)
set( VD_CORE_DIR                ${VD_SRC_DIR}/core)
set( VD_CONSTANTS_DIR           ${VD_SRC_DIR}/constants)
set( VD_EXTRACTED_DIR           ${VD_SRC_DIR}/constants/extracted)
set( VD_FORMATS_DIR             ${VD_SRC_DIR}/formats)
set( VD_INPUT_DIR               ${VD_SRC_DIR}/input)
set( VD_OUTPUT_DIR              ${VD_SRC_DIR}/output)
set( VD_NETWORK_DIR             ${VD_SRC_DIR}/network)
set( VD_GRAPHICS_DIR            ${VD_SRC_DIR}/graphics)
set( VD_INTERFACE_DIR           ${VD_SRC_DIR}/interface)
set( VD_RENDER_DIR              ${VD_SRC_DIR}/render)
set( VD_RUNTIME_DIR             ${VD_SRC_DIR}/runtime)
set( VD_SCHEMA_DIR              ${VD_SRC_DIR}/schema)
set( VD_SPATIAL_DIR             ${VD_SRC_DIR}/spatial)
set( VD_STORAGE_DIR             ${VD_SRC_DIR}/storage)
set( VD_TEST_DIR                ${VD_SRC_DIR}/test)
set( VD_SYMBOLS_H               "${VD_EXTRACTED_DIR}/symbols.h" )
set( VD_SYMBOLS_CPP             "${VD_EXTRACTED_DIR}/symbols.cpp" )
set( VD_RESOURCES_TXT           "${VD_EXTRACTED_DIR}/resources.txt" )

file( GLOB VD_FRAMEWORK_INC     ${VD_FRAMEWORK_DIR}/*.h ${VD_FRAMEWORK_DIR}/*.hpp)
file( GLOB VD_FRAMEWORK_SRC     ${VD_FRAMEWORK_DIR}/*.c ${VD_FRAMEWORK_DIR}/*.cpp)
file( GLOB VD_CORE_INC          ${VD_CORE_DIR}/*.h ${VD_CORE_DIR}/*.hpp)
file( GLOB VD_CORE_SRC          ${VD_CORE_DIR}/*.c ${VD_CORE_DIR}/*.cpp)
file( GLOB VD_COMPUTE_INC       ${VD_COMPUTE_DIR}/*.h ${VD_COMPUTE_DIR}/*.hpp)
file( GLOB VD_COMPUTE_SRC       ${VD_COMPUTE_DIR}/*.c ${VD_COMPUTE_DIR}/*.cpp)
file( GLOB VD_CONSTANTS_INC     ${VD_CONSTANTS_DIR}/*.h ${VD_CONSTANTS_DIR}/*.hpp)
file( GLOB VD_CONSTANTS_SRC     ${VD_CONSTANTS_DIR}/*.c ${VD_CONSTANTS_DIR}/*.cpp)
file( GLOB VD_FORMATS_INC       ${VD_FORMATS_DIR}/*.h ${VD_FORMATS_DIR}/*.hpp)
file( GLOB VD_FORMATS_SRC       ${VD_FORMATS_DIR}/*.c ${VD_FORMATS_DIR}/*.cpp)
file( GLOB VD_GRAPHICS_INC      ${VD_GRAPHICS_DIR}/*.h ${VD_GRAPHICS_DIR}/*.hpp ${VD_GRAPHICS_DIR}/*/*.hpp)
file( GLOB VD_GRAPHICS_SRC      ${VD_GRAPHICS_DIR}/*.c ${VD_GRAPHICS_DIR}/*.cpp ${VD_GRAPHICS_DIR}/*/*.cpp)
file( GLOB VD_INTERFACE_INC     ${VD_INTERFACE_DIR}/*.h ${VD_INTERFACE_DIR}/*.hpp ${VD_INTERFACE_DIR}/*/*.h)
file( GLOB VD_INTERFACE_SRC     ${VD_INTERFACE_DIR}/*.c ${VD_INTERFACE_DIR}/*.cpp ${VD_INTERFACE_DIR}/*/*.cpp)
file( GLOB VD_NETWORK_INC       ${VD_NETWORK_DIR}/*.h ${VD_NETWORK_DIR}/*.hpp )
file( GLOB VD_NETWORK_SRC       ${VD_NETWORK_DIR}/*.c ${VD_NETWORK_DIR}/*.cpp )
file( GLOB VD_RENDER_INC        ${VD_RENDER_DIR}/*.h ${VD_RENDER_DIR}/*.hpp)
file( GLOB VD_RENDER_SRC        ${VD_RENDER_DIR}/*.c ${VD_RENDER_DIR}/*.cpp)
file( GLOB VD_RUNTIME_INC       ${VD_RUNTIME_DIR}/*.h ${VD_RUNTIME_DIR}/*.hpp ${VD_RUNTIME_DIR}/*/*.h)
file( GLOB VD_RUNTIME_SRC       ${VD_RUNTIME_DIR}/*.c ${VD_RUNTIME_DIR}/*.cpp ${VD_RUNTIME_DIR}/*/*.cpp)
file( GLOB VD_SCHEMA_INC        ${VD_SCHEMA_DIR}/*.h ${VD_SCHEMA_DIR}/*.hpp ${VD_SCHEMA_DIR}/*/*.h)
file( GLOB VD_SCHEMA_SRC        ${VD_SCHEMA_DIR}/*.c ${VD_SCHEMA_DIR}/*.cpp ${VD_SCHEMA_DIR}/*/*.cpp)
file( GLOB VD_SPATIAL_INC       ${VD_SPATIAL_DIR}/*.h ${VD_SPATIAL_DIR}/*.hpp )
file( GLOB VD_SPATIAL_SRC       ${VD_SPATIAL_DIR}/*.c ${VD_SPATIAL_DIR}/*.cpp )
file( GLOB VD_STORAGE_INC       ${VD_STORAGE_DIR}/*.h ${VD_STORAGE_DIR}/*.hpp )
file( GLOB VD_STORAGE_SRC       ${VD_STORAGE_DIR}/*.c ${VD_STORAGE_DIR}/*.cpp )
file( GLOB VD_TEST_INC          ${VD_TEST_DIR}/*.h ${VD_TEST_DIR}/*.hpp )
file( GLOB VD_TEST_SRC          ${VD_TEST_DIR}/*.c ${VD_TEST_DIR}/*.cpp )

set( VD_ALL_SRC 
    ${VD_FRAMEWORK_SRC} 
    ${VD_CORE_SRC} 
    ${VD_COMPUTE_SRC} 
    ${VD_CONSTANTS_SRC} 
    ${VD_FORMATS_SRC} 
    ${VD_GRAPHICS_SRC} 
    ${VD_INTERFACE_SRC} 
    ${VD_NETWORK_SRC}
    ${VD_RENDER_SRC} 
    ${VD_RUNTIME_SRC}
    ${VD_SCHEMA_SRC}
    ${VD_SPATIAL_SRC}
    ${VD_STORAGE_SRC}
    ${VD_TEST_SRC}
)

set( VD_ALL_INC
    ${VD_FRAMEWORK_INC} 
    ${VD_CORE_INC} 
    ${VD_COMPUTE_INC} 
    ${VD_CONSTANTS_INC}  
    ${VD_FORMATS_INC} 
    ${VD_GRAPHICS_INC} 
    ${VD_INTERFACE_INC} 
    ${VD_RENDER_INC} 
    ${VD_NETWORK_INC}
    ${VD_RUNTIME_INC}
    ${VD_SCHEMA_INC}
    ${VD_SPATIAL_INC}
    ${VD_STORAGE_INC}
    ${VD_TEST_INC}
)

add_definitions(${VD_DEFINITIONS})
add_definitions( -DVD_VERSION_MAJOR=${VOID_VERSION_MAJOR} )
add_definitions( -DVD_VERSION_MINOR=${VOID_VERSION_MINOR} )
add_definitions( -DVD_VERSION_PATCH=${VOID_VERSION_PATCH} )
add_definitions( -DVD_VERSION_NAMESPACE=${VOID_VERSION_NAMESPACE} )
add_definitions( -DVD_VERSION_SUFFIX=${VOID_VERSION_SUFFIX} )

if(VOID_BUILD_SHARED_LIBS)
    set( VD_LIBRARY_TYPE SHARED )
    add_definitions( -DVD_BUILD_SHARED_LIBRARY=1 )
else()
    set( VD_LIBRARY_TYPE STATIC )
endif()

if(VD_REVISION)
    add_definitions( -DVD_REVISION="${VD_REVISION}" )
endif()

if(VOID_USE_HALF)
    add_definitions( -DVD_USE_HALF=1 )
endif()

if(VOID_USE_DOUBLE_PRECISION)
    add_definitions( -DVD_USE_DOUBLE_PRECISION=1 )
else()
    add_definitions( -DVD_USE_SINGLE_PRECISION=1 )
endif()

if(VOID_USE_ASMLIB)
    add_definitions( -DVD_USE_ASMLIB=1 )
endif()

if(VOID_USE_BOOST_COLLECTIONS)
    add_definitions( -DVD_USE_BOOST_COLLECTIONS=1 )
endif()

if(VOID_USE_OPENGL)
    add_definitions( -DVD_USE_OPENGL=1 )
    add_definitions( -DGLEW_STATIC=1 )
endif()

if(VOID_USE_GLUT)
    add_definitions( -DVD_USE_GLUT_RUNTIME=1 )
    add_definitions( -DVD_USE_GLUT=1 )
endif()

if(VOID_USE_GLFW)
    add_definitions( -DVD_USE_GLFW_RUNTIME=1 )
    add_definitions( -DVD_USE_GLFW=1 )
endif()

if(VOID_USE_GWEN)
    add_definitions( -DVD_USE_GWEN=1 )
endif()

if(VOID_USE_SKIA)
    add_definitions( -DVD_USE_SKIA=1 )
endif()

if(VOID_USE_GS2)
    add_definitions( -DVD_USE_GS2=1 )
endif()

if(VOID_USE_HDF5)
    add_definitions( -DVD_USE_HDF5=1 )
    if(VOID_USE_FLASH)
        add_definitions( -DVD_USE_FLASH=1 )
    endif()
endif()

if(VOID_USE_MSGPACK)
    add_definitions( -DVD_USE_MSGPACK=1 )
endif()

if(VOID_USE_JSON)
    add_definitions( -DVD_USE_JSON=1 )
endif()

if(EXISTS ${VD_SYMBOLS_H})
    add_definitions( -DVD_EXTRACTED_SYMBOLS_EXIST=1 )
endif()

add_definitions( ${VD_DEFINITIONS} )

include_directories(
    ${VD_SRC_DIR}
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
endif()


