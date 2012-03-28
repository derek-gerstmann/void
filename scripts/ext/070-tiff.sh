#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function make_tiff()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    local prefix="$ext_dir/build/$pkg_name/$os_name"

    push_dir "$ext_dir/pkgs/$pkg_base"

    # On OSX disable the tools from getting built since tiffgt.c fails to compile
    if [ "$is_osx" != 0 ]
    then
        rm tools/Makefile
        echo "all:"      >  tools/Makefile
        echo " "         >> tools/Makefile
        echo "install: " >> tools/Makefile
        echo " "         >> tools/Makefile
    fi


    report "Building package '$pkg_name'"
    separator
    make  || bail "Failed to build package: '$prefix'"
    make install  || bail "Failed to build package: '$prefix'"
    separator
    
    pop_dir
}

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
    local existing=$(has_pkg "$pkg_name" "$pkg_opt")

    if [ $existing != 0 ]
    then
        separator
        report "Skipping existing package '$pkg_name' ... "
        return
    fi

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    boot_pkg    $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg
    make_tiff   $pkg_name $pkg_base $pkg_file $pkg_url
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

