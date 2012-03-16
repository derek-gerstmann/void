#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="png"
pkg_base="libpng-1.5.9"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://prdownloads.sourceforge.net/libpng/$pkg_file?download"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_cfg="--disable-shared --enable-static --with-zlib-prefix=$ext_dir/build/zlib/$os_name"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

