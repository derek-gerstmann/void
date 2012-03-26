#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg dependency paths
####################################################################################################

exr_base="$ext_dir/build/exr/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"
szip_base="$ext_dir/build/szip/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
glew_base="$ext_dir/build/glew/$os_name"
tiff_base="$ext_dir/build/tiff/$os_name"
f3d_base="$ext_dir/build/f3d/$os_name"
png_base="$ext_dir/build/png/$os_name"
jpeg_base="$ext_dir/build/jpeg/$os_name"
jp2k_base="$ext_dir/build/jp2k/$os_name"
glew_base="$ext_dir/build/glew/$os_name"
boost_base="$ext_dir/build/boost/$os_name"
mpi_base="$ext_dir/mpi"

mpi_libs="$mpi_base/lib/$os_name/libmpi.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libmpi_cxx.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libmpi_f77.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libmpi_f90.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libompitrace.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libopen-rte.a"
mpi_libs="$mpi_libs $mpi_base/lib/$os_name/libopen-pal.a"

hdf_libs="$hdf_base/lib/libhdf5.a"
hdf_libs="$hdf_libs $hdf_base/lib/libhdf5_hl.a"
hdf_libs="$hdf_libs $zlib_base/lib/libz.a"
hdf_libs="$hdf_libs $szip_base/lib/libsz.a"
hdf_libs="$hdf_libs $mpi_libs"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

export ILMBASE_HOME="$exr_base"
export ILMBASE_VERSION=1.0.3
export ILMBASE_INCLUDE_DIR="$exr_base/include"
export ILMBASE_HALF_LIBRARIES="$exr_base/lib/$os_name/libHalf.a"
export ILMBASE_IEX_LIBRARIES="$exr_base/lib/$os_name/libIex.a"
export ILMBASE_IMATH_LIBRARIES="$exr_base/lib/$os_name/libImath.a"
export ILMBASE_ILMTHREAD_LIBRARIES="$exr_base/lib/$os_name/libIlmThread.a"

export OPENEXR_HOME="$exr_base"
export OPENEXR_INCLUDE_DIR="$exr_base/include/OpenEXR"
export OPENEXR_ILMIMF_LIBRARIES="$exr_base/lib/$os_name/libIlmImf.a $exr_base/lib/$os_name/libHalf.a $exr_base/lib/$os_name/libIex.a $exr_base/lib/$os_name/libImath.a $exr_base/lib/$os_name/libIlmThread.a $zlib_base/lib/$os_name/libz.a"
export OPENEXR_ILMIMF_LIBRARY="$exr_base/lib/libIlmImf.a"

export HDF5_DIR="$hdf_base"
export HDF5_INCLUDE_DIRS="$hdf_base/include"
export HDF5_LIBRARIES="$hdf_libs"
export HDF5_CXX_COMPILER_EXECUTABLE="$hdf_base/bin/h5c++"
export HDF5_C_COMPILER_EXECUTABLE="$hdf_base/bin/h5cc"
export HDF5_C_INCLUDE_DIR="$hdf_base/include"
export HDF5_hdf5_LIBRARY_RELEASE="$hdf_base/lib/libhdf5.a"
export HDF5_hdf5_hl_LIBRARY_RELEASE="$hdf_base/lib/libhdf5_hl.a"
export HDF5_z_LIBRARY_RELEASE="$zlib_base/lib/libz.a"
export HDF5_sz_LIBRARY_RELEASE="$szip_base/lib/libsz.a"

export FIELD3D_HOME="$f3d_base"
export FIELD3D_INCLUDE_DIR="$f3d_base/include/Field3D"
export FIELD3D_LIBRARY="$f3d_base/lib/libField3D.a"

export GLEW_INCLUDE_DIR="$glew_base/include"
export GLEW_LIBRARIES="$glew_base/lib/libGLEW.a"

export JPEG_INCLUDE_DIR="$jpeg_base/include"
export JPEG_LIBRARY="$jpeg_base/lib/libjpeg.a"

export OPENJPEG_INCLUDE_DIR="$jp2k_base/include"
export OPENJPEG_OPENJPEG_LIBRARIES="$jp2k_base/lib/libopenjpeg.a"
export OPENJPEG_LIBRARY="$jp2k_base/lib/libopenjpeg.a"

export TIFF_INCLUDE_DIR="$tiff_base/include"
export TIFF_LIBRARY="$tiff_base/lib/libtiff.a"

export PNG_PNG_INCLUDE_DIR="$png_base/include"
export PNG_LIBRARY="$png_base/lib/libpng.a"

export ZLIB_INCLUDE_DIR="$zlib_base/include"
export ZLIB_LIBRARY="$zlib_base/lib/libz.a"

export BOOST_ROOT="$boost_base"
export Boost_INCLUDE_DIR="$boost_base/include"
export Boost_LIBRARY_DIRS="$boost_base/lib"
export Boost_FILESYSTEM_LIBRARY="$boost_base/lib/libboost_filesystem.a"
export Boost_PYTHON_LIBRARY="$boost_base/lib/libboost_python.a"
export Boost_REGEX_LIBRARY="$boost_base/lib/libboost_regex.a"
export Boost_SYSTEM_LIBRARY="$boost_base/lib/libboost_system.a"
export Boost_THREAD_LIBRARY="$boost_base/lib/libboost_thread.a"
export Boost_PROGRAM_OPTIONS_LIBRARY="$boost_base/lib/libboost_program_options.a"

pkg_name="oiio"
pkg_base="OpenImageIO-oiio-659a244"
pkg_file="OpenImageIO-oiio-Release-1.0.1-659a244.zip"
pkg_url="http://github.com/OpenImageIO/oiio/zipball/Release-1.0.1"
pkg_opt="cmake:keep"

pkg_mpath="$zlib_base"
pkg_mpath="$pkg_mpath:$exr_base"
pkg_mpath="$pkg_mpath:$glew_base"
pkg_mpath="$pkg_mpath:$zlib_base"
pkg_mpath="$pkg_mpath:$szip_base"
pkg_mpath="$pkg_mpath:$hdf_base"
pkg_mpath="$pkg_mpath:$tiff_base"
pkg_mpath="$pkg_mpath:$f3d_base"
pkg_mpath="$pkg_mpath:$png_base"
pkg_mpath="$pkg_mpath:$jpeg_base"
pkg_mpath="$pkg_mpath:$jp2k_base"
pkg_mpath="$pkg_mpath:$glew_base"
pkg_mpath="$pkg_mpath:$boost_base"

pkg_env="-DBUILDSTATIC=ON"
pkg_env="$pkg_env:-DLINKSTATIC=ON"
pkg_env="$pkg_env:-DEMBEDPLUGINS=ON"
pkg_env="$pkg_env:-DUSE_OPENGL=ON"
pkg_env="$pkg_env:-DUSE_PYTHON=OFF"
pkg_env="$pkg_env:-DUSE_FIELD3D=ON"
pkg_env="$pkg_env:-DUSE_OPENJPEG=ON"

pkg_env="$pkg_env:-DILMBASE_HOME=$ilm_base"
pkg_env="$pkg_env:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_env="$pkg_env:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/libHalf.a"
pkg_env="$pkg_env:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/libIex.a"
pkg_env="$pkg_env:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/libImath.a"
pkg_env="$pkg_env:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/libIlmThread.a"

pkg_env="$pkg_env:-DOPENEXR_HOME=$ilm_base"
pkg_env="$pkg_env:-DOPENEXR_INCLUDE_DIR=$ilm_base/include/OpenEXR"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARIES=$ilm_base/lib/libIlmImf.a\;$ilm_base/lib/libHalf.a\;$ilm_base/lib/libIex.a\;$ilm_base/lib/libImath.a\;$ilm_base/lib/libIlmThread.a\;$zlib_base/lib/libz.a"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARY=$ilm_base/lib/libIlmImf.a"

pkg_env="$pkg_env:-DHDF5_HOME=$hdf_base"
pkg_env="$pkg_env:-DHDF5_LIBRARY_DIRS=$hdf_base/lib"
pkg_env="$pkg_env:-DHDF5_INCLUDE_DIRS=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_libs"
pkg_env="$pkg_env:-DHDF5_CXX_COMPILER_EXECUTABLE=$hdf_base/bin/h5c++"
pkg_env="$pkg_env:-DHDF5_C_COMPILER_EXECUTABLE=$hdf_base/bin/h5cc"
pkg_env="$pkg_env:-DHDF5_C_INCLUDE_DIR=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_hdf5_LIBRARY_RELEASE=$hdf_base/lib/libhdf5.a"
pkg_env="$pkg_env:-DHDF5_hdf5_hl_LIBRARY_RELEASE=$hdf_base/lib/libhdf5_hl.a"
pkg_env="$pkg_env:-DHDF5_z_LIBRARY_RELEASE=$zlib_base/lib/libz.a"
pkg_env="$pkg_env:-DHDF5_sz_LIBRARY_RELEASE=$szip_base/lib/libsz.a"

pkg_env="$pkg_env:-DFIELD3D_HOME=$f3d_base"
pkg_env="$pkg_env:-DFIELD3D_INCLUDE_DIR=$f3d_base/include/Field3D"
pkg_env="$pkg_env:-DFIELD3D_LIBRARY=$f3d_base/lib/libField3D.a"

pkg_env="$pkg_env:-DGLEW_INCLUDE_DIR=$glew_base/include"
pkg_env="$pkg_env:-DGLEW_LIBRARIES=$glew_base/lib/libGLEW.a"

pkg_env="$pkg_env:-DJPEG_INCLUDE_DIR=$jpeg_base/include"
pkg_env="$pkg_env:-DJPEG_LIBRARY=$jpeg_base/lib/libjpeg.a"

pkg_env="$pkg_env:-DOPENJPEG_INCLUDE_DIR=$jp2k_base/include"
pkg_env="$pkg_env:-DOPENJPEG_OPENJPEG_LIBRARIES=$jp2k_base/lib/libopenjpeg.a"
pkg_env="$pkg_env:-DOPENJPEG_LIBRARY=$jp2k_base/lib/libopenjpeg.a"

pkg_env="$pkg_env:-DTIFF_INCLUDE_DIR=$tiff_base/include"
pkg_env="$pkg_env:-DTIFF_LIBRARY=$tiff_base/lib/libtiff.a"

pkg_env="$pkg_env:-DPNG_PNG_INCLUDE_DIR=$png_base/include"
pkg_env="$pkg_env:-DPNG_LIBRARY=$png_base/lib/libpng.a"

pkg_env="$pkg_env:-DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_env="$pkg_env:-DZLIB_LIBRARY=$zlib_base/lib/libz.a"

pkg_env="$pkg_env:-DBoost_INCLUDE_DIR=$boost_base/include"
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib"
pkg_env="$pkg_env:-DBoost_FILESYSTEM_LIBRARY=$boost_base/lib/libboost_filesystem.a"
pkg_env="$pkg_env:-DBoost_PYTHON_LIBRARY=$boost_base/lib/libboost_python.a"
pkg_env="$pkg_env:-DBoost_REGEX_LIBRARY=$boost_base/lib/libboost_regex.a"
pkg_env="$pkg_env:-DBoost_SYSTEM_LIBRARY=$boost_base/lib/libboost_system.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/libboost_thread.a"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/libboost_program_options.a"

#env_flags='-DCMAKE_MODULE_PATH="'$pkg_mpath'" -DCMAKE_PREFIX_PATH="'$pkg_mpath'"'

pkg_cfg=" "
#--disable-shared --enable-static"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_mpath $pkg_env $pkg_cfg


