#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="mpi"
pkg_base="openmpi-1.5.4"
pkg_file="$pkg_base.tar.bz2"
pkg_url="http://www.open-mpi.org/software/ompi/v1.5/downloads/$pkg_file"
pkg_cfg="--disable-shared --enable-static --enable-btl-openib-failover --enable-heterogeneous --enable-mpi-thread-multiple"
if [ "$is_osx" -eq 1 ]
then
    # Building v1.5.4 on OSX causes error due to missing '__builtin_expect' -- disable vampire trace avoids this
    pkg_cfg="$pkg_cfg --disable-vt "
fi

pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg

