#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkg_name="glut"
pkg_base="freeglut-2.8.0"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://aarnet.dl.sourceforge.net/project/freeglut/freeglut/2.8.0/$pkg_file"
pkg_cfg="--disable-shared --enable-static"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_keep=1

# build and install pkg into external folder
build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg
