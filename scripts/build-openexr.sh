#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkgname="exr"
pkgbase="OpenEXR"
pkgfile="OpenEXR-savannah-v1.7.x.zip"
pkgurl="http://local.ivec.uwa.edu.au/~derek/files/dev/$pkgfile"
pkgcfg="--disable-shared --enable-static --disable-ilmbasetest CXXFLAGS=-I$extdir/exr/include/OpenEXR LDFLAGS=-L$extdir/exr/lib/$osname "

# build and install pkg into external folder
source "./build-pkg.sh"

