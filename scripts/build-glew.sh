#!/bin/bash

# source common configuration settings
source "./common.sh"

# setup pkg definition and resource files
pkgname="glew"
pkgbase="glew-nightly"
pkgfile="$pkgbase.tar.gz"
pkgurl="git://glew.git.sourceforge.net/gitroot/glew/glew"

# build and install pkg into external folder
source "./build-pkg.sh"

