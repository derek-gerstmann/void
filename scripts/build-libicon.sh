#!/bin/bash

# setup pkg definition and resource files
pkgname="iconv"
pkgbase="libiconv-1.14"
pkgfile="$pkgbase.tar.gz"
pkgurl="http://ftp.gnu.org/pub/gnu/libiconv/$pkgfile"
pkgcfg="--disable-shared --enable-static"

# build and install pkg into external folder
source "./build-pkg.sh"
