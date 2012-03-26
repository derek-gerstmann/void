#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkg_name="dsm"
pkg_base="h5fddsm-0.9.7p1"
pkg_file="h5fdsm-0.9.7p1.tar.bz2"
pkg_url="http://hpcforge.org/frs/download.php/35/$pkg_file"
pkg_cfg=" "
#--disable-shared --enable-static --with-pthread=/usr --with-szlib=$ext_dir/build/szip/$os_name --with-zlib=$ext_dir/build/zlib/$os_name --enable-threadsafe"
pkg_opt="keep"

pkg_mpaths="$ext_dir/build/tiff/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/boost/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/zlib/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/szip/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/mpi/$os_name"
pkg_mpaths="$pkg_mpaths:$ext_dir/build/vfd/$os_name"

mpi_base="$ext_dir/build/mpi/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"
szip_base="$ext_dir/build/szip/$os_name"

pkg_env="-DMAKESTATIC=1:-DLINKSTATIC=1"
pkg_env="$pkg_env:-DMPI_INCLUDE_PATH=$mpi_base/include"
pkg_env="$pkg_env:-DMPI_LIBRARY=$mpi_base/lib/libmpi.a"
pkg_env="$pkg_env:-DMPI_EXTRA_LIBRARY=$mpi_base/lib/libmpi_cxx.a"

pkg_env="$pkg_env:-DHDF5_DIR=$hdf_base/share/cmake/hdf5"
pkg_env="$pkg_env:-DHDF5_CXX_COMPILER_EXECUTABLE=$hdf_base/bin/h5c++"
pkg_env="$pkg_env:-DHDF5_C_COMPILER_EXECUTABLE=$hdf_base/bin/h5cc"
pkg_env="$pkg_env:-DHDF5_C_INCLUDE_DIR=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_base/lib/libhdf5.a;"
pkg_env="$pkg_env:-DHDF5_hdf5_LIBRARY=$hdf_base/lib/libhdf5.a"
pkg_env="$pkg_env:-DHDF5_hdf5_hl_LIBRARY=$hdf_base/lib/libhdf5_hl.a"
pkg_env="$pkg_env:-DHDF5_z_LIBRARY=$zlib_base/lib/libz.a"
pkg_env="$pkg_env:-DHDF5_sz_LIBRARY=$szip_base/lib/libsz.a"
pkg_env="$pkg_env:-DHDF5_ENABLE_PARALLEL=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_ZLIB=ON"
pkg_env="$pkg_env:-DHDF5_ENABLE_HL_LIB=ON"


####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_mpaths $pkg_env $pkg_cfg


