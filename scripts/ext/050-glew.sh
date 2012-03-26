#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function boot_glew()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    push_dir "$ext_dir/pkgs/$pkg_base/auto"
    separator
    make || bail "Failed to boostrap Glew library!"
    separator
    pop_dir
}

function make_glew()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    # build and install into local path
    prefix="$ext_dir/build/$pkg_name/$os_name"
    push_dir "$ext_dir/pkgs/$pkg_base"

    report "Building package '$pkg_name'"
    separator
    make GLEW_DEST="$prefix" || bail "Failed to build package: '$prefix'"
    make GLEW_DEST="$prefix" install || bail "Failed to build package: '$prefix'"
    separator

    pop_dir

}

function build_glew()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5
    local pkg_cflags=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

#    echo "PkgName: '$pkg_name' PkgBase: '$pkg_base' PkgFile: '$pkg_file' PkgUrl: '$pkg_url' PkgCfg: '$pkg_cfg' PkgKeep: '$pkg_keep'"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    boot_glew   $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cflags $pkg_ldflags $pkg_cfg
    make_glew   $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="glew"
pkg_base="glew-nightly"
pkg_file="$pkg_base.tar.bz2"
pkg_url="git://glew.git.sourceforge.net/gitroot/glew/glew"

pkg_opt="keep"
pkg_ldflags=0
pkg_cflags=0
pkg_cfg=0

####################################################################################################
# build and install pkg into external folder using custom GLEW build methods
####################################################################################################

build_glew $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

