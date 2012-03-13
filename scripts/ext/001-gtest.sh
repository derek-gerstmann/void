#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="gtest"
pkg_base="gtest-nightly"
pkg_file="$pkg_base.tar.bz2"
pkg_url="svn://googletest.googlecode.com/svn/trunk"
pkg_cfg="--disable-shared --enable-static"
pkg_ldflags=0
pkg_cflags=0
pkg_keep=1

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cflags $pkg_ldflags $pkg_cfg


