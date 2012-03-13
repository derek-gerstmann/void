#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="exr"
pkg_base="IlmBase"
pkg_file="IlmBase-savannah-v1.7.x.zip"
pkg_url="http://local.ivec.uwa.edu.au/~derek/files/dev/$pkg_file"
pkg_cfg="--disable-shared --enable-static"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg

