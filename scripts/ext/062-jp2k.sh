#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="jp2k"
pkg_base="openjpeg-1.5.0"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://openjpeg.googlecode.com/files/$pkg_file"
pkg_cfg="--disable-shared --enable-static "
pkg_cfg="$pkg_cfg PNG_CFLAGS=-I$ext_dir/build/png/$os_name/include"
pkg_cfg="$pkg_cfg PNG_LIBS=-lpng"
pkg_cfg="$pkg_cfg Z_CFLAGS=-I$ext_dir/build/zlib/$os_name/include"
pkg_cfg="$pkg_cfg Z_LIBS=-lz"
pkg_cfg="$pkg_cfg TIFF_CFLAGS=-I$ext_dir/build/tiff/$os_name/include/tiff"
pkg_cfg="$pkg_cfg TIFF_LIBS=-ltiff"
pkg_cflags="-I$ext_dir/build/png/$os_name/include:-I$ext_dir/build/zlib/$os_name/include:-I$ext_dir/build/tiff/$os_name/include:-I$ext_dir/build/tiff/$os_name/include/tiff"
pkg_ldflags="-L$ext_dir/build/png/$os_name/lib:-L$ext_dir/build/zlib/$os_name/lib:-L$ext_dir/build/tiff/$os_name/lib"
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg
