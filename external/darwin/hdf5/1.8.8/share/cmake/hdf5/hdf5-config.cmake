#-----------------------------------------------------------------------------
# HDF5 Config file for compiling against hdf5 install directory
#-----------------------------------------------------------------------------
GET_FILENAME_COMPONENT (SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${SELF_DIR}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
GET_FILENAME_COMPONENT(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

#-----------------------------------------------------------------------------
# User Options
#-----------------------------------------------------------------------------
SET (HDF5_ENABLE_PARALLEL OFF)
SET (HDF5_BUILD_FORTRAN   OFF)
SET (HDF5_ENABLE_F2003    )
SET (HDF5_BUILD_CPP_LIB   ON)
SET (HDF5_BUILD_TOOLS     OFF)
SET (HDF5_BUILD_HL_LIB    OFF)
SET (HDF5_ENABLE_Z_LIB_SUPPORT ON)
SET (HDF5_ENABLE_SZIP_SUPPORT  ON)
SET (HDF5_ENABLE_SZIP_ENCODING OFF)
SET (HDF5_BUILD_SHARED_LIBS    OFF)
SET (HDF5_PACKAGE_EXTLIBS      OFF)

#-----------------------------------------------------------------------------
# Directories
#-----------------------------------------------------------------------------
SET (HDF5_INCLUDE_DIR "${_IMPORT_PREFIX}/include" )

IF (HDF5_BUILD_FORTRAN)
  SET (HDF5_INCLUDE_DIR_FORTRAN "${_IMPORT_PREFIX}/include/fortran" )
ENDIF (HDF5_BUILD_FORTRAN)
  
IF (HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_CPP "${_IMPORT_PREFIX}/include/cpp" )
ENDIF (HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_HL_LIB)
  SET (HDF5_INCLUDE_DIR_HL "${_IMPORT_PREFIX}/include/hl" )
ENDIF (HDF5_BUILD_HL_LIB)

IF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)
  SET (HDF5_INCLUDE_DIR_HL_CPP "${_IMPORT_PREFIX}/include/hl/cpp" )
ENDIF (HDF5_BUILD_HL_LIB AND HDF5_BUILD_CPP_LIB)

IF (HDF5_BUILD_TOOLS)
  SET (HDF5_INCLUDE_DIR_TOOLS "${_IMPORT_PREFIX}/include/tools" )
  SET (HDF5_TOOLS_DIR "${_IMPORT_PREFIX}/bin/tools" )
ENDIF (HDF5_BUILD_TOOLS)

#-----------------------------------------------------------------------------
# Version Strings
#-----------------------------------------------------------------------------
SET (HDF5_VERSION_STRING 1.8.8)
SET (HDF5_VERSION_MAJOR  1.8)
SET (HDF5_VERSION_MINOR  8)

#-----------------------------------------------------------------------------
# Don't include targets if this file is being picked up by another
# project which has already built hdf5 as a subproject
#-----------------------------------------------------------------------------
IF (NOT TARGET "hdf5")
  IF (HDF5_ENABLE_Z_LIB_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "zlib")
    INCLUDE (${SELF_DIR}/../ZLIB/ZLIB-targets.cmake)
  ENDIF (HDF5_ENABLE_Z_LIB_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "zlib")
  IF (HDF5_ENABLE_SZIP_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "szip")
    INCLUDE (${SELF_DIR}/../SZIP/SZIP-targets.cmake)
  ENDIF (HDF5_ENABLE_SZIP_SUPPORT AND HDF5_PACKAGE_EXTLIBS AND NOT TARGET "szip")
  INCLUDE (${SELF_DIR}/hdf5-targets.cmake)
  SET (HDF5_LIBRARIES "hdf5;hdf5_cpp")
ENDIF (NOT TARGET "hdf5")

#-----------------------------------------------------------------------------
# Unfinished
#-----------------------------------------------------------------------------
#
# To be continued (maybe) ...
#
# XXX_INCLUDE_DIRS         no, use one per library as in HDF5_FORTRAN_INCLUDE_DIR
# XXX_LIBRARIES             not needed - see hdf5-targets.cmake
# XXX_DEFINITIONS           Definitions to use when compiling code that uses XXX. This really shouldn't include options such as (-DHAS_JPEG) that a client source-code file uses to decide whether to #include <jpeg.h>
# XXX_EXECUTABLE            Where to find the XXX tool.
# XXX_YYY_EXECUTABLE        Where to find the YYY tool that comes with XXX.
# XXX_LIBRARY_DIRS         Optionally, the final set of library directories listed in one variable for use by client code.  This should not be a cache entry.
# XXX_ROOT_DIR              Where to find the base directory of XXX.
# XXX_VERSION_YY           Expect Version YY if true. Make sure at most one of these is ever true.
# XXX_WRAP_YY               If False, do not try to use the relevent CMake wrapping command.
# XXX_YY_FOUND              If False, optional YY part of XXX sytem is not available.
# XXX_FOUND                 Set to false, or undefined, if we haven't found, or don't want to use XXX.
# XXX_RUNTIME_LIBRARY_DIRS Optionally, the runtime library search path for use when running an executable linked to shared libraries.
#                          The list should be used by user code to create the PATH on windows or LD_LIBRARY_PATH on unix.
#                          This should not be a cache entry.
