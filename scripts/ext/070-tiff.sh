#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function build_tiff()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_opt=$5
    local pkg_cflags=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    boot_pkg    $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

# On OSX disable the tools from getting built since tiffgt.c fails to compile
if [ "$is_osx" != 0 ]
then
    push_dir "$ext_dir/pkgs/$pkg_base"
    rm tools/Makefile
    echo "all:"      >  tools/Makefile
    echo " "         >> tools/Makefile
    echo "install: " >> tools/Makefile
    echo " "         >> tools/Makefile
    pop_dir
fi

    make_pkg    $pkg_name $pkg_base $pkg_file $pkg_url
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="tiff"
pkg_base="tiff-4.0.1"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://download.osgeo.org/libtiff/$pkg_file"
pkg_opt="configure:keep"

pkg_cflags="-I$ext_dir/build/png/$os_name/include:-I$ext_dir/build/zlib/$os_name/include"
pkg_ldflags="-L$ext_dir/build/png/$os_name/lib:-L$ext_dir/build/zlib/$os_name/lib"

pkg_cfg="--disable-shared --enable-static --with-gnu-ld"
pkg_cfg="$pkg_cfg --with-jpeg-lib-dir=$ext_dir/build/jpeg/$os_name/lib"
pkg_cfg="$pkg_cfg --with-jpeg-include-dir=$ext_dir/build/jpeg/$os_name/include"
pkg_cfg="$pkg_cfg --with-zlib-lib-dir=$ext_dir/build/zlib/$os_name/lib"
pkg_cfg="$pkg_cfg --with-zlib-include-dir=$ext_dir/build/zlib/$os_name/include"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_tiff $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

