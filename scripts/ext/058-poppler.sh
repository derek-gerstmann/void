#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="poppler"
pkg_base="poppler-0.18.4"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://poppler.freedesktop.org/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include:-I$ext_dir/png/include:-I$ext_dir/iconv/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-L$ext_dir/png/lib/$os_name:-L$ext_dir/iconv/lib/$os_name:-liconv"
pkg_cfg="--disable-shared --enable-static"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

