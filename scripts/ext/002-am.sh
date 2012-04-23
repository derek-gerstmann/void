#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="am"
pkg_base="automake-1.11.5"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://ftp.gnu.org/gnu/automake/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags=0
pkg_ldflags=0
pkg_cfg="--disable-shared --enable-static"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

# setup the environment to support our own version of AUTOMAKE
if [ -d "$ext_dir/build/am/$os_name" ]
then
    if ![ -d "$ext_dir/build/am/$os_name/share/aclocal" ]
    then
        ln -s "$ext_dir/build/am/$os_name/share/aclocal-1.11" "$ext_dir/build/am/$os_name/share/aclocal" 
    fi
fi
