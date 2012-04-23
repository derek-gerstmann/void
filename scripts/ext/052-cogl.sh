#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="cogl"
pkg_base="cogl-1.10.2"
pkg_file="$pkg_base.tar.bz2"
pkg_url="http://source.clutter-project.org/sources/cogl/1.10/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/png/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/iconv/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/gettext/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/gettext/share/gettext"
pkg_cflags="$pkg_cflags:-I$ext_dir/glib/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/glib/include/glib-2.0"
pkg_cflags="$pkg_cflags:-I$ext_dir/glib/include/gio-unix-2.0"

pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-lz"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/png/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/iconv/lib/$os_name:-liconv"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/glib/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-lglib-2.0:-lgio-2.0:-lgobject-2.0:-lgmodule-2.0:-lgthread-2.0"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/gettext/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-lasprintf:-lgettextpo:-lintl"

pkg_cfg="--disable-glibtest --disable-shared --enable-static --with-libiconv-prefix=$ext_dir/build/iconv/$os_name --with-libintl-prefix=$ext_dir/build/gettext/$os_name"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

