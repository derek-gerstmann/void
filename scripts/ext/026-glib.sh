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
pkg_cflags="-I$ext_dir/zlib/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/icu/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/iconv/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/libffi/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/gettext/include"
pkg_cflags="$pkg_cflags:-I$ext_dir/gettext/share/gettext"

pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-lz"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/libffi/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-lffi"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/icu/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-licudata:-licui18n:-licuio:-licule:-liculx:-licutest:-licutu:-licuuc"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/iconv/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-liconv"
pkg_ldflags="$pkg_ldflags:-L$ext_dir/gettext/lib/$os_name"
pkg_ldflags="$pkg_ldflags:-lasprintf:-lgettextpo"

if [ "$is_osx" -eq 1 ]
then
    pkg_ldflags="$pkg_ldflags:-lintl"
fi

pkg_cfg="--disable-glibtest --disable-shared --enable-static"
pkg_cfg="$pkg_cfg --with-libiconv-prefix=$ext_dir/build/iconv/$os_name"
pkg_cfg="$pkg_cfg --with-libintl-prefix=$ext_dir/build/gettext/$os_name"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

