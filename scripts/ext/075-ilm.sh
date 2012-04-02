#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="exr"
pkg_base="exr/IlmBase"
pkg_file="OpenEXR-github-v1.7.x.tar.bz2"
pkg_url="git://github.com/openexr/openexr.git"

pkg_opt="configure:keep"
pkg_cflags=0
pkg_ldflags=0
pkg_cfg="--disable-shared --enable-static --disable-ilmbasetest CXXFLAGS=-I/usr/include"

####################################################################################################

function make_exr()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    # build and install into local path
    prefix="$ext_dir/build/$pkg_name/$os_name"
    push_dir "$ext_dir/pkgs/$pkg_base"

	echo "#include <stdint.h>" >> config/IlmBaseConfig.h

    report "Building package '$pkg_name'"
    separator
    make || bail "Failed to build package: '$prefix'"
    separator

    pop_dir
}

function build_exr()
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
    make_exr    $pkg_name $pkg_base $pkg_file $pkg_url 
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_exr $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

