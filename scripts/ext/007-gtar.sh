#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="gtar"
pkg_base="tar-1.26"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://ftp.gnu.org/gnu/tar/tar-1.26.tar.gz"

pkg_opt="configure:keep"
pkg_cflags=0
pkg_ldflags=0
pkg_cfg="--disable-shared --enable-static --with-xz=$ext_dir/xz/bin/$os_name/xz --with-lzip=$ext_dir/zlib/lib/$os_name/libz.a --with-bzip2=$ext_dir/bzip/bin/$os_name/bzip2"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg


