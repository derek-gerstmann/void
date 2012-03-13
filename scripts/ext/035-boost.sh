#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function make_boost()
{
    # configure package
    local m=7
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_cmkpaths=$5
    local pkg_envflags=$6
    local pkg_cfg="${@:$m}"

    cd "$pkg_base"
    separator
  
    prefix="$ext_dir/build/$pkg_name/$os_name"
    env_flags=" "
    if [ -n $pkg_cmkpaths ] && [ $pkg_cmkpaths != 0 ]
    then
        pkg_cmkpaths=$(echo $pkg_cmkpaths | split_str ":" | join_str " ")
    	env_flags='-DCMAKE_MODULE_PATH="'$pkg_cmkpaths'"'
    fi
        
    if [ -n $pkg_envflags ] && [ $pkg_envflags != 0 ]
    then
        pkg_envflags=$(echo $pkg_envflags | split_str ":" | join_str " ")
    	env_flags=$env_flags' '$pkg_envflags
    fi

    report "Configuring package '$pkg_name' ..."
    separator
    echo ./bootstrap.sh --prefix="$prefix"
    separator
    eval ./bootstrap.sh --prefix="$prefix" || bail "Failed to configure: '$prefix'"
    separator
    eval ./b2 --prefix="$prefix" $env_flags  || bail "Failed to build: '$prefix'"
    separator
    eval ./b2 install || bail "Failed to install: '$prefix'"
    separator
    report "Done building and installing package '$pkg_name'"
}

function build_boost()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5
    local pkg_cmkpaths=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

#    echo "PkgName: '$pkg_name' PkgBase: '$pkg_base' PkgFile: '$pkg_file' PkgUrl: '$pkg_url' PkgCfg: '$pkg_cfg' PkgKeep: '$pkg_keep'"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    make_boost  $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cmkpaths $pkg_ldflags $pkg_cfg
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="boost"
pkg_base="boost_1_49_0"
pkg_file="boost_1_49_0.tar.bz2"
pkg_url="http://sourceforge.net/projects/boost/files/boost/1.49.0/boost_1_49_0.tar.bz2/download"
pkg_cfg="--disable-shared --enable-static"
pkg_cmkpaths=0
pkg_envflags="link=static;threading=single;threading=multi "
pkg_keep=1

####################################################################################################
# build and install pkg into external folder using custom BOOST build methods
####################################################################################################

build_boost $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cmkpaths $pkg_envflags $pkg_cfg


