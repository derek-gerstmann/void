#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="rsvg"
pkg_base="librsvg-2.36.0"
pkg_file="librsvg-2.36.0.tar.xz"
pkg_url="http://download.gnome.org/sources/librsvg/2.36/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_cfg="--disable-shared --enable-static --with-zlib-prefix=$ext_dir/build/zlib/$os_name --with-pkgconfigdir=$PKG_CONFIG_PATH"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

