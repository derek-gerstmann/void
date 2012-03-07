#!/bin/bash

# setup project paths
abscwd="$( cd "$( dirname "$0" )" && pwd )"
basedir="$( basename "$abscwd" )"
rootdir="$( dirname "$abscwd" )"
extdir="$rootdir/external"

# setup pkg definition and resource files
osname=
pkgname="iconv"
pkgbase="libiconv-1.14"
pkgfile="$pkgbase.tar.gz"
pkgurl="http://ftp.gnu.org/pub/gnu/libiconv/$pkgfile"

# ensure we are run inside of the scripts folder
if [ "$basedir" != "scripts" ]
then
	echo "Please execute from within the void/scripts subfolder!"
	exit 0
fi 

# move into external pkg folder for retrieving pkgs
cd "$extdir"
mkdir -p pkgs
cd pkgs

# remove any existing extracted pkg folder
if [ -d "$pkgbase" ]
then
	rm -r "$pkgbase"
fi

# if a local copy doesn't exist, grab the pkg from the url
if [ ! -f "$pkgfile" ]
then
	wget "$pkgurl"
fi

# extract pkg file and move into extracted folder
tar -zxvf "$pkgfile"
cd "$pkgbase"

# configure package
./configure --prefix="$extdir/$pkgname" --disable-shared --enable-statics

# build and install into local path
make
make install

# move libraries into os path
if [ -d "$extdir/$pkgname/lib" ]
then
	mv "$extdir/$pkgname/lib" "$extdir/$pkgname/lnx" 
	mkdir -p "$extdir/$pkgname/lib"
	mv "$extdir/$pkgname/lnx" "$extdir/$pkgname/lib"
fi

# move binaries into os path
if [ -d "$extdir/$pkgname/bin" ]
then
	mv "$extdir/$pkgname/bin" "$extdir/$pkgname/lnx" 
	mkdir -p "$extdir/$pkgname/bin"
	mv "$extdir/$pkgname/lnx" "$extdir/$pkgname/bin"
fi

