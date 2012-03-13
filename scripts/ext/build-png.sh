#!/bin/bash

# source common configuration settings
source "./common.sh"
source "./build-pkg.sh"

# setup pkg definition and resource files
pkg_name="png"
pkg_base="libpng-1.5.9"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://prdownloads.sourceforge.net/libpng/$pkg_file?download"
pkg_cfg="--disable-shared --enable-static --with-zlib-prefix=$ext_dir/build/zlib/$os_name LDFLAGS=-L$ext_dir/build/zlib/$os_name/lib CFLAGS=-I$ext_dir/build/zlib/$os_name/include"
pkg_keep=1

# build and install pkg into external folder
build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cfg

