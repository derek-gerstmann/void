#!/bin/bash

pkgname="glut"
pkgbase="freeglut-2.8.0"
pkgfile="$pkgbase.tar.gz"
pkgurl="http://aarnet.dl.sourceforge.net/project/freeglut/freeglut/2.8.0/$pkgfile"
pkgcfg="--disable-shared --enable-static"

# build and install pkg into external folder
source "./build-pkg.sh"

