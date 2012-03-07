find_library( EXPAT_LIBRARY expat )
find_library( BZIP2_LIBRARY bz2 )

set( VD_PLATFORM_LIBS 
	${EXPAT_LIBRARY} 
	${BZIP2_LIBRARY} 
)

file( GLOB VD_PLATFORM_SRC 
    ${VD_SRC_DIR}/*/${VD_SYSTEM_DIR}/*.c 
    ${VD_SRC_DIR}/*/${VD_SYSTEM_DIR}/*.cpp
)

if(VD_SYSTEM_OSX)

    find_library( COCOA_LIBRARY Cocoa )

	include_directories(/System/Library/Frameworks/OpenGL.framework/Headers)
	find_path(OPENGL_INCLUDE_DIR OpenGL/gl3.h)

	set( VD_OSX_ICON_FILES ${VD_ROOT_DIR}/resources/platform/osx/Void.icns ) 
	set_source_files_properties( ${VD_OSX_ICON_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	
    file( GLOB VD_PLATFORM_SRC 
        ${VD_SRC_DIR}/*/${VD_SYSTEM_DIR}/*.m 
        ${VD_SRC_DIR}/*/${VD_SYSTEM_DIR}/*.c 
        ${VD_SRC_DIR}/*/${VD_SYSTEM_DIR}/*.cpp
    	${VD_OSX_ICON_FILES}
    )
    
    set( VD_PLATFORM_LIBS ${COCOA_LIBRARY}
        /System/Library/Frameworks/AppKit.framework
        /System/Library/Frameworks/Foundation.framework
        /System/Library/Frameworks/Carbon.framework
        /System/Library/Frameworks/OpenGL.framework
        /System/Library/Frameworks/IOKit.framework
        ${VD_PLATFORM_LIBS} 
	)
endif()

if(VD_SYSTEM_WIN)
    source_group( "Shaders" FILES ${VD_SHADERS} )
    source_group( "Build" FILES CMakeLists.txt )
    add_definitions( /wd4996 )

    file( GLOB VD_PLATFORM_SRC ${VD_SRC_DIR}/*/win/*.c ${VD_SRC_DIR}/*/win/*.cpp)
    set( VD_PLATFORM_LIBS opengl32 WIN32        
    	${VD_PLATFORM_LIBS} 
	)
endif()

if( ${VD_PLATFORM_SRC} )
    add_library( VdPlatform ${VD_PLATFORM_SRC} )
    target_link_libraries( VdPlatform ${VD_PLATFORM_LIBS} )
endif()



