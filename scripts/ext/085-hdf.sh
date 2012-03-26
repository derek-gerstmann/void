#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkg_name="hdf"
pkg_base="hdf5-1.8.8"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://www.hdfgroup.org/ftp/HDF5/current/src/$pkg_file"

zlib_base="$ext_dir/zlib"
szip_base="$ext_dir/szip"
mpi_base="$ext_dir/mpi"
tiff_base="$ext_dir/tiff"
vfd_base="$ext_dir/vfd"

pkg_mpaths="$zlib_base:$szip_base:$tiff_base:$mpi_base:$vfd_base"

pkg_env="-DMAKESTATIC=1"
pkg_env="$pkg_env:-DLINKSTATIC=1"

pkg_env="$pkg_env:-DMPI_INCLUDE_PATH=$mpi_base/include"
pkg_env="$pkg_env:-DMPI_LIBRARY=$mpi_base/lib/libmpi.a"
pkg_env="$pkg_env:-DMPI_EXTRA_LIBRARY=$mpi_base/lib/libopen-pal.a"

pkg_env="$pkg_env:-DSZIP_INCLUDE_DIR=$szip_base/include"
pkg_env="$pkg_env:-DSZIP_LIBRARY=$szip_base/lib/$os_name/libsz.a"

pkg_env="$pkg_env:-DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_env="$pkg_env:-DZLIB_LIBRARY=$zlib_base/lib/$os_name/libz.a"

# Silly HDF5: parallel (aka wrapped in thread-safe mutex locks) and C++ support are mutually exclusive
# pkg_env="$pkg_env:-DHDF5_ENABLE_PARALLEL=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_ZLIB=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_Z_LIB_SUPPORT=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_ZLIB_SUPPORT=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_SZIP_SUPPORT=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_HL_LIB=ON"
pkg_env="$pkg_env:-DHDF5_BUILD_CPP_LIB=ON"
pkg_env="$pkg_env:-DHDF5_DISABLE_COMPILER_WARNINGS=ON"

pkg_cfg="-DCMAKE_PREFIX_PATH=$ext_dir"
pkg_cfg="$pkg_cfg -DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_cfg="$pkg_cfg -DZLIB_LIBRARY=$zlib_base/lib/$os_name/libz.a"
pkg_cfg="$pkg_cfg -DSZIP_INCLUDE_DIR=$szip_base/include"
pkg_cfg="$pkg_cfg -DSZIP_LIBRARY=$szip_base/lib/$os_name/libsz.a"

pkg_opt="cmake:keep"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_mpaths $pkg_env $pkg_cfg


