#!/bin/bash

# setup pkg definition and resource files
pkgname="icu"
pkgbase="icu/source"
pkgfile="icu4c-49_rc-src.tgz"
pkgurl="http://download.icu-project.org/files/icu4c/49rc/$pkgfile"
pkgcfg="--disable-shared --enable-static"

# build and install pkg into external folder
source "./build-pkg.sh"

