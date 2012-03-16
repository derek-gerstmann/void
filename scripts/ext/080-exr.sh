#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="exr"
pkg_base="OpenEXR"
pkg_file="OpenEXR-savannah-v1.7.x.zip"
pkg_url="http://local.ivec.uwa.edu.au/~derek/files/dev/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/exr/include/OpenEXR"
pkg_ldflags="-L$ext_dir/exr/lib/$os_name"
pkg_cfg="--disable-shared --enable-static --disable-ilmbasetest"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

