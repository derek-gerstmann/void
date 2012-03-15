#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="oiio"
pkg_base="OpenImageIO-oiio-6441966"
pkg_file="OpenImageIO-oiio-Release-1.0.0-19-gcc5028d.tar.gz"
pkg_url="http://nodeload.github.com/OpenImageIO/oiio/tarball/RB-1.0"
pkg_cfg="--disable-shared --enable-static"

ilm_base="$ext_dir/build/exr/$os_name"
exr_base="$ext_dir/build/exr/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"
szip_base="$ext_dir/build/szip/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
tiff_base="$ext_dir/build/tiff/$os_name"
f3d_base="$ext_dir/build/f3d/$os_name"
png_base="$ext_dir/build/png/$os_name"
jpeg_base="$ext_dir/build/jpeg/$os_name"
jp2k_base="$ext_dir/build/jp2k/$os_name"
glew_base="$ext_dir/build/glew/$os_name"
boost_base="$ext_dir/build/boost/$os_name"

pkg_mpath="$zlib_base"
pkg_mpath="$pkg_mpath:$ilm_base"
pkg_mpath="$pkg_mpath:$exr_base"
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

pkg_env="-DBUILDSTATIC=1:-DLINKSTATIC=1:-DUSE_PYTHON=OFF"

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
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_base/lib/libhdf5.a\;$hdf_base/lib/libhdf5_hl.a\;$zlib_base/lib/libz.a\;$szip_base/lib/libsz.a"

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
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib/$os_name"
pkg_env="$pkg_env:-DBoost_FILESYSTEM_LIBRARY=$boost_base/lib/libboost_filesystem.a"
pkg_env="$pkg_env:-DBoost_PYTHON_LIBRARY=$boost_base/lib/libboost_python.a"
pkg_env="$pkg_env:-DBoost_REGEX_LIBRARY=$boost_base/lib/libboost_regex.a"
pkg_env="$pkg_env:-DBoost_SYSTEM_LIBRARY=$boost_base/lib/libboost_system.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/libboost_thread.a"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/libboost_program_options.a"

pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_mpath $pkg_env $pkg_cfg


