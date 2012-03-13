#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkg_name="szip"
pkg_base="szip-2.1"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://www.hdfgroup.org/ftp/lib-external/szip/2.1/src/$pkg_file"
pkg_cfg="--disable-shared --enable-static"
pkg_ldflags=0
pkg_cflags=0
pkg_keep=1

# build and install pkg into external folder
build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg


