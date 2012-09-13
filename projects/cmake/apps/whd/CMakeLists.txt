cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
if(VOID_USE_GLUT)

project( Whd )

set( VD_APP_NAME                        Whd )
set( VD_APP_WHD_DIR                     ${VD_APPS_DIR}/whd)
set( VD_APP_WHD_SRC_DIR                 ${VD_APP_WHD_DIR}/source)
set( VD_APP_WHD_DATA_DIR                ${VD_APP_WHD_DIR}/data)
set( VD_APP_WHD_SYMBOLS_H               ${VD_APP_WHD_DATA_DIR}/constants/symbols.h )
set( VD_APP_WHD_SYMBOLS_CPP             ${VD_APP_WHD_DATA_DIR}/constants/symbols.cpp )
set( VD_APP_WHD_RESOURCES               ${VD_APP_WHD_DATA_DIR}/resources/${VD_APP_NAME}Resources.txt )

file( GLOB VD_APP_WHD_SRC               ${VD_APP_WHD_SRC_DIR}/*.c ${VD_APP_WHD_SRC_DIR}/*.cpp)
file( GLOB VD_APP_WHD_INC               ${VD_APP_WHD_SRC_DIR}/*.h ${VD_APP_WHD_SRC_DIR}/*.hpp)
file( GLOB VD_APP_WHD_FONTS             ${VD_APP_WHD_DATA_DIR}/fonts/*.ttf ${VD_APP_WHD_DATA_DIR}/fonts/*.otf )
file( GLOB VD_APP_WHD_SHADERS           ${VD_APP_WHD_DATA_DIR}/shaders/*.* )
file( GLOB VD_APP_WHD_TEXTURES          ${VD_APP_WHD_DATA_DIR}/textures/*.* )

set( VD_APP_WHD_ALL_RSC                 ${VD_APP_WHD_FONTS} ${VD_APP_WHD_SHADERS} ${VD_APP_WHD_TEXTURES} )

find_library( GUI_LIBRARY Glut )
    
set(VD_CXX_STD_FLAGS ${VD_CXX_STD_FLAGS} "-fno-rtti")

set( VD_APP_WHD_ALL_SRC 
    ${VD_ALL_SRC} 
    ${VD_APP_WHD_SRC}
)

set( VD_APP_WHD_ALL_INC
    ${VD_ALL_INC} 
    ${VD_APP_WHD_INC}
)

include_directories(
    ${VD_APPS_DIR}
    ${VD_APP_WHD_DIR}
    ${VD_APP_WHD_DATA_DIR}
    ${VD_INTERFACE_DIR}/glut
    ${VD_INTERFACE_DIR}/gwen
)

set( VD_APP_WHD_SYMEX_FILELIST      ${VD_APP_WHD_ALL_SRC} ${VD_APP_WHD_ALL_INC} ${VD_PLATFORM_SRC} )

vd_extract_symbols(
    ${VD_APP_WHD_SYMBOLS_H} ${VD_APP_WHD_SYMBOLS_CPP} 
    VD_CONSTANTS_EXTRACTED_WHD_SYMBOLS_SCOPE 
    VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE
    Whd
    ${VD_APP_WHD_SYMEX_FILELIST})

add_executable( Whd WIN32 MACOSX_BUNDLE
    ${VD_APP_WHD_SRC} 
    ${VD_APP_WHD_INC} 
    ${VD_APP_WHD_SYMBOLS_H} 
    ${VD_APP_WHD_SYMBOLS_CPP} )

target_link_libraries( Whd 
    VdExternal 
    VdFramework 
    VdPlatform 
    VdSymbols
    ${GUI_LIBRARY}
)

add_dependencies( Whd 
    ${VD_APP_WHD_ALL_SRC} 
    ${VD_APP_WHD_FONTS} 
    ${VD_APP_WHD_SHADERS} 
    ${VD_APP_WHD_TEXTURES} 
    ${VD_APP_WHD_RESOURCES} )

add_custom_target( WhdResources ALL DEPENDS 
    ${VD_APP_WHD_FONTS} 
    ${VD_APP_WHD_SHADERS} 
    ${VD_APP_WHD_TEXTURES} 
    ${VD_APP_WHD_RESOURCES} )

if(APPLE)

    set(MACOSX_BUNDLE_INFO_STRING "${VD_APP_NAME}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.icrar.${VD_APP_NAME}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VD_APP_NAME} Version ${VOID_VERSION_STRING}")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${VD_APP_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VOID_SHORT_VERSION_STRING})
    set(MACOSX_BUNDLE_BUNDLE_VERSION 1.0)
    set(MACOSX_BUNDLE_BUNDLE_ICON "Void.icns")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2012. All Rights Reserved.")
    
    # These variables are specific to our plist and are NOT standard CMake variables
    set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
    set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")

    find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
    if(${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
      message(SEND_ERROR "ibtool can not be found")
    endif()

    SET_TARGET_PROPERTIES(
        Whd PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST
        ${VD_ROOT_DIR}/resources/platform/osx/Info.plist
    )

    add_custom_command (
        OUTPUT ${VD_APP_WHD_RESOURCES} 
        COMMENT "Creating Resources Directory ..."
        COMMAND ${CMAKE_COMMAND} -E make_directory Whd.app/Contents/Resources
        COMMENT "Copying Resources to Bundle ..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${VD_APP_WHD_DATA_DIR}/fonts Whd.app/Contents/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${VD_APP_WHD_DATA_DIR}/shaders Whd.app/Contents/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${VD_APP_WHD_DATA_DIR}/textures Whd.app/Contents/Resources
        COMMAND ${CMAKE_COMMAND} -E touch ${VD_APP_WHD_RESOURCES}
        DEPENDS ${VD_APP_WHD_ALL_SRC} ${VD_APP_WHD_ALL_RSC} 
    )

    add_custom_command (
        TARGET Whd
        POST_BUILD 
        COMMENT "Building MainMenu NIB for ${VD_APP_NAME}..."
        COMMAND mkdir -p Whd.app/Contents/Resources
        COMMAND ${IBTOOL} --errors --warnings --notices
        --output-format human-readable-text 
        --compile Whd.app/Contents/Resources/MainMenu.nib 
         ${VD_ROOT_DIR}/resources/platform/osx/MainMenu.xib
        DEPENDS ${VD_APP_WHD_RESOURCES}
    )

endif()

message(STATUS "Configured: ${VD_APP_NAME}")

endif()

