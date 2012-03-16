#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

zlib_base="$ext_dir/zlib"
png_base="$ext_dir/png"
tiff_base="$ext_dir/tiff"

pkg_name="jp2k"
pkg_base="openjpeg-1.5.0"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://openjpeg.googlecode.com/files/$pkg_file"

pkg_opt="configure:keep"

pkg_cflags="-I$zlib_base/include"
pkg_cflags="$pkg_cflags:-I$png_base/include"
pkg_cflags="$pkg_cflags:-I$tiff_base/include"
pkg_cflags="$pkg_cflags:-I$tiff_base/include/tiff"

pkg_ldflags="-L$zlib_base/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-L$png_base/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-L$tiff_base/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-L$tiff_base/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-L$tiff_base/lib/$os_name/libtiff.a"
pkg_ldflags="$pkg_ldflags:-L$tiff_base/lib/$os_name/libtiffxx.a"

pkg_cfg="--disable-shared --enable-static "
pkg_cfg="$pkg_cfg Z_CFLAGS=-I$zlib_base/include"
pkg_cfg="$pkg_cfg Z_LIBS=-lz"
pkg_cfg="$pkg_cfg PNG_CFLAGS=-I$png_base/include"
pkg_cfg="$pkg_cfg PNG_LIBS=-lpng"
pkg_cfg="$pkg_cfg TIFF_CFLAGS=-I$tiff_base/include"
pkg_cfg="$pkg_cfg TIFF_LIBS=$tiff_base/lib/$os_name/libtiff.a"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg
