#----------------------------------------------------------------
# Generated CMake target import file for configuration "".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Compute the installation prefix relative to this file.
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Import target "hdf5" for configuration ""
SET_PROPERTY(TARGET hdf5 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "m;/Users/dg/Workspace/void/clean/void/external/zlib/lib/osx/libz.a;/Users/dg/Workspace/void/clean/void/external/szip/lib/osx/libsz.a"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libhdf5.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS hdf5 )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_hdf5 "${_IMPORT_PREFIX}/lib/libhdf5.a" )

# Import target "hdf5_cpp" for configuration ""
SET_PROPERTY(TARGET hdf5_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
SET_TARGET_PROPERTIES(hdf5_cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "hdf5"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libhdf5_cpp.a"
  )

LIST(APPEND _IMPORT_CHECK_TARGETS hdf5_cpp )
LIST(APPEND _IMPORT_CHECK_FILES_FOR_hdf5_cpp "${_IMPORT_PREFIX}/lib/libhdf5_cpp.a" )

# Loop over all imported files and verify that they actually exist
FOREACH(target ${_IMPORT_CHECK_TARGETS} )
  FOREACH(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    IF(NOT EXISTS "${file}" )
      MESSAGE(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    ENDIF()
  ENDFOREACH()
  UNSET(_IMPORT_CHECK_FILES_FOR_${target})
ENDFOREACH()
UNSET(_IMPORT_CHECK_TARGETS)

# Cleanup temporary variables.
SET(_IMPORT_PREFIX)

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)
