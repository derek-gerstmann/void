#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="fc"
pkg_base="fontconfig-2.8.0"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://www.freedesktop.org/software/fontconfig/release/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include:-I$ext_dir/iconv/include:-I$ext_dir/ft/include:-I$ext_dir/xml2/include:-I$ext_dir/xml2/include/libxml2"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-L$ext_dir/iconv/lib/$os_name:-liconv:-L$ext_dir/ft/lib/$os_name:-lfreetype:-L$ext_dir/xml2/lib/$os_name:-lxml2"
pkg_cfg="--disable-shared --enable-static --with-freetype-config=$ext_dir/ft/bin/$os_name/freetype-config LIBXML2_CFLAGS=-I$ext_dir/xml2/include LIBXML2_LIBS=$ext_dir/xml2/lib/$os_name/libxml2.a"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

