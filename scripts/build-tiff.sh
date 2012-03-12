#!/bin/bash

# source common configuration settings
source "./common.sh"
source "./build-pkg.sh"

# setup pkg definition and resource files
pkg_name="tiff"
pkg_base="tiff-4.0.1"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://download.osgeo.org/libtiff/$pkg_file"
pkg_cfg="--disable-shared --enable-static --with-gnu-ld"
pkg_cfg="$pkg_cfg --with-jpeg-lib-dir=$ext_dir/build/jpeg/$os_name/lib"
pkg_cfg="$pkg_cfg --with-jpeg-include-dir=$ext_dir/build/jpeg/$os_name/include"
pkg_cfg="$pkg_cfg --with-zlib-lib-dir=$ext_dir/build/zlib/$os_name/lib"
pkg_cfg="$pkg_cfg --with-zlib-include-dir=$ext_dir/build/zlib/$os_name/include"
pkg_ldflags="-L$ext_dir/build/png/$os_name/lib:-L$ext_dir/build/zlib/$os_name/lib:-L$ext_dir/build/tiff/$os_name/lib"
pkg_cflags="-I$ext_dir/build/png/$os_name/include:-I$ext_dir/build/zlib/$os_name/include:-I$ext_dir/build/tiff/$os_name/include:-I$ext_dir/build/tiff/$os_name/include/tiff"
pkg_keep=1

# build and install pkg into external folder
build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg

