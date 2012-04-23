#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="glib"
pkg_base="glib-2.32.1"
pkg_file="$pkg_base.tar.xz"
pkg_url="http://ftp.gnome.org/pub/GNOME/sources/glib/2.32/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include:-I$ext_dir/iconv/include:-I$ext_dir/gettext/include:-I$ext_dir/gettext/share/gettext"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-L$ext_dir/iconv/lib/$os_name:-liconv:-L$ext_dir/gettext/lib/$os_name:-lintl:-lgettextpo:-lasprintf"
pkg_cfg="--disable-shared --enable-static"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

