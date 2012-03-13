#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="oiio"
pkg_base="OpenImageIO-oiio-cc5028d"
pkg_file="OpenImageIO-oiio-Release-1.0.0-19-gcc5028d.tar.gz"
pkg_url="http://nodeload.github.com/OpenImageIO/oiio/tarball/RB-1.0"
pkg_cfg="--disable-shared --enable-static"

ilm_base="$ext_dir/exr"
exr_base="$ext_dir/exr"
zlib_base="$ext_dir/zlib"
szip_base="$ext_dir/szip"
hdf_base="$ext_dir/hdf"
tiff_base="$ext_dir/tiff"
f3d_base="$ext_dir/f3d"
png_base="$ext_dir/png"
jpeg_base="$ext_dir/jpeg"
jp2k_base="$ext_dir/jp2k"
glew_base="$ext_dir/glew"
boost_base="$ext_dir/boost"

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
pkg_env="$pkg_env:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/$os_name/libHalf.a"
pkg_env="$pkg_env:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/$os_name/libIex.a"
pkg_env="$pkg_env:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/$os_name/libImath.a"
pkg_env="$pkg_env:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/$os_name/libIlmThread.a"

pkg_env="$pkg_env:-DOPENEXR_HOME=$ilm_base"
pkg_env="$pkg_env:-DOPENEXR_INCLUDE_DIR=$ilm_base/include/OpenEXR"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARIES=$ilm_base/lib/$os_name/libIlmImf.a\;$ilm_base/lib/$os_name/libHalf.a\;$ilm_base/lib/$os_name/libIex.a\;$ilm_base/lib/$os_name/libImath.a\;$ilm_base/lib/$os_name/libIlmThread.a\;$zlib_base/lib/$os_name/libz.a"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARY=$ilm_base/lib/$os_name/libIlmImf.a"

pkg_env="$pkg_env:-DHDF5_HOME=$hdf_base"
pkg_env="$pkg_env:-DHDF5_LIBRARY_DIRS=$hdf_base/lib"
pkg_env="$pkg_env:-DHDF5_INCLUDE_DIRS=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_base/lib/$os_name/libhdf5.a\;$hdf_base/lib/$os_name/libhdf5_hl.a\;$zlib_base/lib/$os_name/libz.a\;$szip_base/lib/$os_name/libsz.a"

pkg_env="$pkg_env:-DFIELD3D_HOME=$f3d_base"
pkg_env="$pkg_env:-DFIELD3D_INCLUDE_DIR=$f3d_base/include/Field3D"
pkg_env="$pkg_env:-DFIELD3D_LIBRARY=$f3d_base/lib/$os_name/libField3D.a"

pkg_env="$pkg_env:-DGLEW_INCLUDE_DIR=$glew_base/include"
pkg_env="$pkg_env:-DGLEW_LIBRARIES=$glew_base/lib/$os_name/libGLEW.a"

pkg_env="$pkg_env:-DJPEG_INCLUDE_DIR=$jpeg_base/include"
pkg_env="$pkg_env:-DJPEG_LIBRARY=$jpeg_base/lib/$os_name/libjpeg.a"

pkg_env="$pkg_env:-DOPENJPEG_INCLUDE_DIR=$jp2k_base/include"
pkg_env="$pkg_env:-DOPENJPEG_OPENJPEG_LIBRARIES=$jp2k_base/lib/$os_name/libopenjpeg.a"
pkg_env="$pkg_env:-DOPENJPEG_LIBRARY=$jp2k_base/lib/$os_name/libopenjpeg.a"

pkg_env="$pkg_env:-DTIFF_INCLUDE_DIR=$tiff_base/include"
pkg_env="$pkg_env:-DTIFF_LIBRARY=$tiff_base/lib/$os_name/libtiff.a"

pkg_env="$pkg_env:-DPNG_PNG_INCLUDE_DIR=$png_base/include"
pkg_env="$pkg_env:-DPNG_LIBRARY=$png_base/lib/$os_name/libpng.a"

pkg_env="$pkg_env:-DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_env="$pkg_env:-DZLIB_LIBRARY=$zlib_base/lib/$os_name/libz.a"

pkg_env="$pkg_env:-DBoost_INCLUDE_DIR=$boost_base/include"
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib/$os_name"
pkg_env="$pkg_env:-DBoost_FILESYSTEM_LIBRARY=$boost_base/lib/$os_name/libboost_filesystem.a"
pkg_env="$pkg_env:-DBoost_PYTHON_LIBRARY=$boost_base/lib/$os_name/libboost_python.a"
pkg_env="$pkg_env:-DBoost_REGEX_LIBRARY=$boost_base/lib/$os_name/libboost_regex.a"
pkg_env="$pkg_env:-DBoost_SYSTEM_LIBRARY=$boost_base/lib/$os_name/libboost_system.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/$os_name/libboost_thread.a"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/$os_name/libboost_program_options.a"

pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_mpath $pkg_env $pkg_cfg


