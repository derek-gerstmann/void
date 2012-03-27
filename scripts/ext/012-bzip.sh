#!/bin/bash

# source common configuration settings
source "./common.sh"

#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function make_bzip()
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
    make PREFIX="$prefix" || bail "Failed to build package: '$prefix'"
    make PREFIX="$prefix" install || bail "Failed to build package: '$prefix'"
    separator

    pop_dir

}

function build_bzip()
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

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cflags $pkg_ldflags $pkg_cfg
    make_bzip   $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="bzip"
pkg_base="bzip2-1.0.6"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://www.bzip.org/1.0.6/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags=0
pkg_ldflags=0
pkg_cfg="--disable-shared --enable-static"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_bzip $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg


