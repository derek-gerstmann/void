#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="msgpack"
pkg_base="msgpack-0.5.7"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://msgpack.org/releases/cpp/$pkg_file"
pkg_cfg="--disable-shared --enable-static"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg

