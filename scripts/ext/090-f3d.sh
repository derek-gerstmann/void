#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="f3d"
pkg_base="imageworks-Field3D-94d1bf3"
pkg_file="imageworks-Field3D-v1.3.2-0-gcac7b57.tar.gz"
pkg_url="http://github.com/imageworks/Field3D/tarball/v1.3.2"
pkg_cfg="--disable-shared --enable-static"
pkg_keep=1

pkg_mpaths="$ext_dir/build/tiff/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/boost/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/zlib/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/szip/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/png/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/jpeg/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/jp2k/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/hdf/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/glew/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/ilm/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/exr/$os_name"

ilm_base="$ext_dir/build/exr/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"
szip_base="$ext_dir/build/szip/$os_name"
boost_base="$ext_dir/boost"

pkg_env="-DMAKESTATIC=1:-DLINKSTATIC=1"

pkg_env="$pkg_env:-DIlmbase_Base_Dir=$ilm_base"
pkg_env="$pkg_env:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_env="$pkg_env:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/libHalf.a"
pkg_env="$pkg_env:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/libIex.a"
pkg_env="$pkg_env:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/libImath.a"
pkg_env="$pkg_env:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/libIlmThread.a"

pkg_env="$pkg_env:-DBoost_INCLUDE_DIR=$boost_base/include"
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib/$os_name"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/$os_name/libboost_program_options.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/$os_name/libboost_thread.a"

pkg_env="$pkg_env:-DHDF5_HOME=$hdf_base"
pkg_env="$pkg_env:-DHDF5_CXX_COMPILER_EXECUTABLE=$hdf_base/bin/h5c++"
pkg_env="$pkg_env:-DHDF5_C_COMPILER_EXECUTABLE=$hdf_base/bin/h5cc"
pkg_env="$pkg_env:-DHDF5_C_INCLUDE_DIR=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_base/lib/libhdf5.a"
pkg_env="$pkg_env:-DHDF5_hdf5_LIBRARY=$hdf_base/lib/libhdf5.a"
pkg_env="$pkg_env:-DHDF5_hdf5_hl_LIBRARY=$hdf_base/lib/libhdf5_hl.a"
pkg_env="$pkg_env:-DHDF5_z_LIBRARY=$zlib_base/lib/libz.a"
pkg_env="$pkg_env:-DHDF5_sz_LIBRARY=$szip_base/lib/libsz.a"

pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################
build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_mpaths $pkg_env $pkg_cfg


