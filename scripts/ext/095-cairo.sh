#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="cairo"
pkg_base="cairo-1.11.4"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://www.cairographics.org/snapshots/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include:-I$ext_dir/png/include:-I$ext_dir/iconv/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-L$ext_dir/png/lib/$os_name:-L$ext_dir/iconv/lib/$os_name:-liconv"
pkg_cfg="--disable-shared --enable-static"

if [ "$is_lnx" -eq 1 ]
then
    pkg_cfg="$pkg_cfg --enable-drm --enable-directfb --enable-gl --enable-cogl"
fi

if [ "$is_osx" -eq 1 ]
then
    pkg_cfg="$pkg_cfg --enable-quartz --enable-quartz-image"
    pkg_ldflags="$pkg_ldflags:-framework:ApplicationServices"
fi

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

