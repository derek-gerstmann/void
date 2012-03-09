#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkgname="exr"
pkgbase="IlmBase"
pkgfile="IlmBase-savannah-v1.7.x.zip"
pkgurl="http://local.ivec.uwa.edu.au/~derek/files/dev/$pkgfile"
pkgcfg="--disable-shared --enable-static"
pkgkeep=1

# build and install pkg into external folder
source "./build-pkg.sh"

