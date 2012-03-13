#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="ft"
pkg_base="freetype-2.4.9"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://downloads.sourceforge.net/project/freetype/freetype2/2.4.9/freetype-2.4.9.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Ffreetype%2Ffiles%2Ffreetype2%2F2.4.9%2F&ts=1331269502&use_mirror=aarnet"
pkg_cfg="--disable-shared --enable-static"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg

