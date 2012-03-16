cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
if(VOID_USE_GLUT)

project( GuiDemo )

set( VD_APP_NAME                        GuiDemo )
set( VD_APP_GUI_DEMO_DIR                ${VD_APPS_DIR}/gui)

set( VD_APPS_GUI_DEMO_SYMBOLS_H         "${VD_APP_GUI_DEMO_DIR}/symbols.h" )
set( VD_APPS_GUI_DEMO_RESOURCES_TXT     "${VD_APP_NAME}Resources.txt" )

file( GLOB VD_APPS_GUI_DEMO_SRC         ${VD_APP_GUI_DEMO_DIR}/*.c ${VD_APP_GUI_DEMO_DIR}/*.cpp)
file( GLOB VD_APPS_GUI_DEMO_INC         ${VD_APP_GUI_DEMO_DIR}/*.h ${VD_APP_GUI_DEMO_DIR}/*.hpp)

file( GLOB VD_FONTS                     ${VD_RESOURCE_DIR}/fonts/*.ttf ${VD_RESOURCE_DIR}/fonts/*.otf )
file( GLOB VD_SHADERS                   ${VD_RESOURCE_DIR}/shaders/*.glsl )
file( GLOB VD_TEXTURES                  ${VD_RESOURCE_DIR}/textures/*.* )

find_library( GUI_LIBRARY Glut )

set(VD_CXX_STD_FLAGS ${VD_CXX_STD_FLAGS} "-fno-rtti")
    
set( VD_APPS_GUI_DEMO_ALL_SRC 
    ${VD_ALL_SRC} 
    ${VD_APPS_GUI_DEMO_SRC}
)

set( VD_APPS_GUI_DEMO_ALL_INC
    ${VD_ALL_INC} 
    ${VD_APPS_GUI_DEMO_INC}
)

include_directories(
    ${VD_APPS_DIR}
    ${VD_APPS_DIR}/gui
    ${VD_INTERFACE_DIR}/glut
    ${VD_INTERFACE_DIR}/gwen
)

if(VOID_USE_EXTRACTED_SYMBOLS)
    set( VD_APPS_GUI_SYMEX_FILELIST ${VD_APPS_GUI_DEMO_ALL_SRC} ${VD_APPS_GUI_DEMO_ALL_INC} ${VD_PLATFORM_SRC} )
    set( VD_APPS_GUI_DEMO_ALL_SRC ${VD_APPS_GUI_DEMO_ALL_SRC} ${VD_APPS_GUI_DEMO_SYMBOLS_H} )
    
    extract_symbols(
        ${VD_APPS_GUI_DEMO_SYMBOLS_H} 
        VD_CONSTANTS_EXTRACTED_DEMO_SYMBOLS_SCOPE 
        VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
        Demo
        ${VD_APPS_GUI_SYMEX_FILELIST})

    add_custom_target(VdSymExDemoHeader ALL DEPENDS ${VD_APPS_GUI_SYMEX_FILELIST} ${VD_ALL_SRC} ${VD_ALL_INC} )
endif()

if(EXISTS ${VD_APPS_GUI_DEMO_SYMBOLS_H})
    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )
endif()
    
add_executable( GuiDemo WIN32 MACOSX_BUNDLE
    ${VD_APPS_GUI_DEMO_SRC} ${VD_APPS_GUI_DEMO_INC} ${VD_APPS_GUI_DEMO_SYMBOLS_H} )

target_link_libraries( GuiDemo 
    VdExternal 
    VdFramework 
    VdPlatform 
    ${VD_EXT_ICONV_LIB}
    ${VD_EXT_LIB}
    ${GUI_LIBRARY}
)

add_dependencies( GuiDemo 
    VdExternal 
    VdFramework 
    VdPlatform 
    ${VD_APPS_GUI_DEMO_ALL_SRC} 
    ${VD_FONTS} 
    ${VD_SHADERS} 
    ${VD_TEXTURES} 
    ${VD_APPS_GUI_DEMO_SYMBOLS_H}
    ${VD_APPS_GUI_DEMO_RESOURCES_TXT} )

add_custom_target( GuiDemoResources ALL DEPENDS 
    ${VD_FONTS} ${VD_SHADERS} ${VD_TEXTURES} ${VD_APPS_GUI_DEMO_RESOURCES_TXT} )

if(VD_SYSTEM_OSX)

    set(MACOSX_BUNDLE_INFO_STRING "${VD_APP_NAME}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.icrar.${VD_APP_NAME}")
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
        GuiDemo PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        ${VD_ROOT_DIR}/resources/platform/osx/Info.plist
    )

    add_custom_command (
        OUTPUT ${VD_APPS_GUI_DEMO_RESOURCES_TXT} 
        COMMENT "Creating Resources Directory ..."
        COMMAND mkdir -p GuiDemo.app/Contents/Resources
        COMMENT "Copying Resources to Bundle ..."
        COMMAND cp ${VD_FONTS} GuiDemo.app/Contents/Resources
        COMMAND cmake -E touch ${VD_APPS_GUI_DEMO_RESOURCES_TXT}
        DEPENDS ${VD_APPS_GUI_DEMO_ALL_SRC} ${VD_FONTS} 
    )

    add_custom_command (
        TARGET GuiDemo
        POST_BUILD 
        COMMENT "Building MainMenu NIB for ${VOID_PROJECT_NAME}..."
        COMMAND mkdir -p GuiDemo.app/Contents/Resources
        COMMAND ${IBTOOL} --errors --warnings --notices
        --output-format human-readable-text 
        --compile GuiDemo.app/Contents/Resources/MainMenu.nib 
         ${VD_ROOT_DIR}/resources/platform/osx/MainMenu.xib
        DEPENDS ${VD_APPS_GUI_DEMO_RESOURCES_TXT}
    )

endif()

message(STATUS "Configured features: ${CMAKE_BUILD_TYPE} ${VOID_VERSION_STRING}")

endif()

