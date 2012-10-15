cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
if(VOID_USE_GLUT)

project( SkinDemo )

set( VD_APP_NAME                        SkinDemo )
set( VD_APPS_SKIN_DEMO_DIR              ${VD_APPS_DIR}/skin)
set( VD_APPS_SKIN_DEMO_SYMBOLS         "${VD_APPS_SKIN_DEMO_DIR}/symbols.h" )
set( VD_APPS_SKIN_DEMO_SYMBOLS_H       "${VD_APPS_SKIN_DEMO_DIR}/symbols.h" )
set( VD_APPS_SKIN_DEMO_SYMBOLS_CPP     "${VD_APPS_SKIN_DEMO_DIR}/symbols.cpp" )
set( VD_APPS_SKIN_DEMO_RESOURCES       "${VD_APP_NAME}Resources.txt" )

file( GLOB VD_APPS_SKIN_DEMO_SRC        ${VD_APPS_SKIN_DEMO_DIR}/*.c ${VD_APPS_SKIN_DEMO_DIR}/*.cpp)
file( GLOB VD_APPS_SKIN_DEMO_INC        ${VD_APPS_SKIN_DEMO_DIR}/*.h ${VD_APPS_SKIN_DEMO_DIR}/*.hpp)

file( GLOB VD_FONTS                     ${VD_RESOURCE_DIR}/fonts/*.ttf ${VD_RESOURCE_DIR}/fonts/*.otf )
file( GLOB VD_SHADERS                   ${VD_RESOURCE_DIR}/shaders/*.glsl )
file( GLOB VD_TEXTURES                  ${VD_RESOURCE_DIR}/textures/*.* )

set(VD_CXX_STD_FLAGS ${VD_CXX_STD_FLAGS} "-fno-rtti")
    
include_directories(
    ${VD_APPS_DIR}
    ${VD_APPS_DIR}/gui
    ${VD_INTERFACE_DIR}/glut
    ${VD_INTERFACE_DIR}/gwen
)

set( VD_APP_SKIN_SYMEX_FILELIST      
    ${VD_APPS_SKIN_DEMO_INC} 
    ${VD_APPS_SKIN_DEMO_SRC} 
    ${VD_PLATFORM_SRC} )

vd_extract_symbols(
    ${VD_APPS_SKIN_DEMO_SYMBOLS_H} ${VD_APPS_SKIN_DEMO_SYMBOLS_CPP} 
    VD_CONSTANTS_EXTRACTED_GUI_SYMBOLS_SCOPE 
    VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
    Demo
    ${VD_APP_SKIN_SYMEX_FILELIST})

add_executable( SkinDemo WIN32 MACOSX_BUNDLE
    ${VD_APPS_SKIN_DEMO_INC} 
    ${VD_APPS_SKIN_DEMO_SRC}
    ${VD_APPS_SKIN_DEMO_SYMBOLS_H}
    ${VD_APPS_SKIN_DEMO_SYMBOLS_CPP} )

target_link_libraries( SkinDemo 
    VdFramework 
    VdPlatform 
    VdSymbols
    ${VD_EXT_ICONV_LIB}
    ${VD_EXT_LIB}
    ${SKIN_LIBRARY}
)

add_dependencies( SkinDemo 
    ${VD_APPS_SKIN_DEMO_ALL_SRC} 
    ${VD_APPS_SKIN_DEMO_FONTS} 
    ${VD_APPS_SKIN_DEMO_SHADERS} 
    ${VD_APPS_SKIN_DEMO_TEXTURES} 
    ${VD_APPS_SKIN_DEMO_RESOURCES} )

add_custom_target( SkinResources ALL DEPENDS 
    ${VD_APPS_SKIN_DEMO_FONTS} 
    ${VD_APPS_SKIN_DEMO_SHADERS} 
    ${VD_APPS_SKIN_DEMO_TEXTURES} 
    ${VD_APPS_SKIN_DEMO_RESOURCES} )

if(VD_SYSTEM_OSX)

    set(MACOSX_BUNDLE_INFO_STRING "${VD_APP_NAME}")
    set(MACOSX_BUNDLE_SKIN_IDENTIFIER "org.icrar.${VD_APP_NAME}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VD_APP_NAME} Version ${VOID_VERSION_STRING}")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${VD_APP_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VOID_SHORT_VERSION_STRING})
    set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0)
    set(MACOSX_BUNDLE_BUNDLE_ICON "Void.icns")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2011. All Rights Reserved.")
    
    # These variables are specific to our plist and are NOT standard CMake variables
    set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
    set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")

    find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
    if(${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
      message(SEND_ERROR "ibtool can not be found")
    endif()

    SET_TARGET_PROPERTIES(
        SkinDemo PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        ${VD_ROOT_DIR}/resources/platform/osx/Info.plist
    )

    add_custom_command (
        OUTPUT ${VD_APPS_SKIN_DEMO_RESOURCES} 
        COMMENT "Creating Resources Directory ..."
        COMMAND mkdir -p SkinDemo.app/Contents/Resources
        COMMENT "Copying Resources to Bundle ..."
        COMMAND cp ${VD_FONTS} SkinDemo.app/Contents/Resources
        COMMAND cmake -E touch ${VD_APPS_SKIN_DEMO_RESOURCES}
        DEPENDS ${VD_APPS_SKIN_DEMO_ALL_SRC} ${VD_FONTS} 
    )

    add_custom_command (
        TARGET SkinDemo
        POST_BUILD 
        COMMENT "Building MainMenu NIB for ${VOID_PROJECT_NAME}..."
        COMMAND mkdir -p SkinDemo.app/Contents/Resources
        COMMAND ${IBTOOL} --errors --warnings --notices
        --output-format human-readable-text 
        --compile SkinDemo.app/Contents/Resources/MainMenu.nib 
         ${VD_ROOT_DIR}/resources/platform/osx/MainMenu.xib
        DEPENDS ${VD_APPS_SKIN_DEMO_RESOURCES}
    )

endif()

message(STATUS "Configured: ${VD_APP_NAME}")

endif()

