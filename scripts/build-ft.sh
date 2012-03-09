#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkgname="ft"
pkgbase="freetype-2.4.9"
pkgfile="$pkgbase.tar.gz"
pkgurl="http://downloads.sourceforge.net/project/freetype/freetype2/2.4.9/freetype-2.4.9.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Ffreetype%2Ffiles%2Ffreetype2%2F2.4.9%2F&ts=1331269502&use_mirror=aarnet"
pkgcfg="--disable-shared --enable-static"
pkgkeep=1

# build and install pkg into external folder
source "./build-pkg.sh"

