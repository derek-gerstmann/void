if(NOT CMAKE_BUILD_TYPE)
  if(VOID_RELEASE)
    set(CMAKE_BUILD_TYPE Release)
  else()
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
    	  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      	FORCE)
  endif()
endif(NOT CMAKE_BUILD_TYPE)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set( VD_SYSTEM_LNX           1 )
    set( VD_SYSTEM_IS_LINUX      1 )
    set( VD_SYSTEM_ACRONYMN      lnx )
    set( VD_SYSTEM_NAME          "Linux" )
    set( VD_SYSTEM_PLATFORM_NAME "linux" )
    set( VD_STATIC_LIB_EXT       a )
    set( VD_SHARED_LIB_EXT       so )
    add_definitions(-fPIC) # 64bit Linux wants this
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set( VD_SYSTEM_OSX           1 )
    set( VD_SYSTEM_IS_OSX        1 )
    set( VD_SYSTEM_ACRONYMN      osx )
    set( VD_SYSTEM_NAME          "Mac OS X" )
    set( VD_SYSTEM_PLATFORM_NAME "darwin" )
    set( VD_STATIC_LIB_EXT       a )
    set( VD_SHARED_LIB_EXT       dylib )
    set( CMAKE_OSX_ARCHITECTURES x86_64 CACHE PATH "" FORCE) # Force 64bit to be default
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set( VD_SYSTEM_WINDOWS       1 )
    set( VD_SYSTEM_IS_WINDOWS    1 )
    set( VD_SYSTEM_ACRONYMN      win )
    set( VD_SYSTEM_NAME          "Windows" )
    set( VD_SYSTEM_PLATFORM_NAME "windows" )
    set( VD_STATIC_LIB_EXT       lib )
    set( VD_SHARED_LIB_EXT       dll )
endif()

set(VD_SYSTEM_DIR ${VD_SYSTEM_ACRONYMN})

message(STATUS "Building on '${VD_SYSTEM_NAME} (${VD_SYSTEM_PLATFORM_NAME})' as '${CMAKE_BUILD_TYPE}'")

include(FindRevision)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set( VD_COMPILER_IS_CLANG  1 )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set( VD_COMPILER_IS_GCC  1 )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set( VD_COMPILER_IS_ICC  1 )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set( VD_COMPILER_IS_MSVC  1 )
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    include(FindCompilerVersion)
    VD_COMPILER_DUMP_VERSION(GCC_COMPILER_VERSION)
    if(GCC_COMPILER_VERSION LESS 41)
        message(ERROR "GCC 4.1 or later required, found ${GCC_COMPILER_VERSION}")
    endif()
endif(CMAKE_COMPILER_IS_GNUCXX)

set(VD_C_INCLUDE_FLAGS "-I/usr/include -I/usr/local/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VD_C_INCLUDE_FLAGS}")

if(VD_COMPILER_IS_GCC)
    set(VD_CXX_STD_FLAGS "${CMAKE_CXX_FLAGS} ${VD_C_INCLUDE_FLAGS}")

    if(CMAKE_BUILD_TYPE MATCHES Release)
        set(VD_CXX_WARN_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wsign-promo -Winit-self -Wno-unused-parameter -Wno-write-strings -Wuninitialized")
        set(VD_CXX_SIMD_FLAGS "-msse -msse2 -msse3 -msse4 -msse4.2 -mfpmath=sse")
    else()
        set(VD_CXX_WARN_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wsign-promo -Winit-self -Wno-unused-parameter -Wno-write-strings")
        set(VD_CXX_SIMD_FLAGS "-msse -msse2 -msse3 -msse4 -msse4.2 -mfpmath=sse")
    endif()

    if(VD_SYSTEM_IS_LINUX)
        set(VD_CXX_SIMD_FLAGS "-march=native ${VD_CXX_SIMD_FLAGS}")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VD_CXX_STD_FLAGS} ${VD_CXX_WARN_FLAGS} ${VD_CXX_SIMD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${VD_CXX_STD_FLAGS} ${VD_CXX_WARN_FLAGS} ${VD_CXX_SIMD_FLAGS}")
endif(VD_COMPILER_IS_GCC)

if(VD_COMPILER_IS_CLANG)
    set(VD_CXX_STD_FLAGS "${CMAKE_CXX_FLAGS} ${VD_C_INCLUDE_FLAGS}")

    if(CMAKE_BUILD_TYPE MATCHES Release)
        set(VD_CXX_WARN_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wsign-promo -Winit-self -Wno-unused-parameter -Wno-write-strings -Wuninitialized")
        set(VD_CXX_SIMD_FLAGS "-msse -msse2 -msse3 -msse4 -msse4.2")
    else()
        set(VD_CXX_WARN_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wsign-promo -Winit-self -Wno-unused-parameter -Wno-write-strings")
        set(VD_CXX_SIMD_FLAGS "-msse -msse2 -msse3 -msse4 -msse4.2")
    endif()

    if(VD_SYSTEM_IS_LINUX)
        set(VD_CXX_SIMD_FLAGS "-march=native ${VD_CXX_SIMD_FLAGS}")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VD_CXX_STD_FLAGS} ${VD_CXX_WARN_FLAGS} ${VD_CXX_SIMD_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${VD_CXX_STD_FLAGS} ${VD_CXX_WARN_FLAGS} ${VD_CXX_SIMD_FLAGS}")
endif(VD_COMPILER_IS_CLANG)

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(VOID_RELEASE ON)
  	set(VD_DEFINITIONS -DVD_RELEASE_BUILD)	
  	set(VERSION_ABI ${VERSION_MAJOR}${VERSION_MINOR}0)
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(VD_DEFINITIONS -DVD_DEBUG_BUILD)
    set(VOID_VERSION_SUFFIX "dev")
endif(CMAKE_BUILD_TYPE MATCHES Debug)


