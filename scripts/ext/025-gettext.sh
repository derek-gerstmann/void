#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="gettext"
pkg_base="gettext-0.18.1.1"
pkg_file="$pkg_base.tar.gz"
pkg_url="http://ftp.gnu.org/pub/gnu/gettext/$pkg_file"

pkg_opt="configure:keep"
pkg_cflags="-I$ext_dir/zlib/include:-I$ext_dir/iconv/include:-I$ext_dir/icu/include"
pkg_ldflags="-L$ext_dir/zlib/lib/$os_name:-L$ext_dir/iconv/lib/$os_name:-L$ext_dir/icu/lib/$os_name:-liconv"
pkg_cfg="--disable-shared --enable-static --with-gnu-ld --without-emacs"
pkg_cfg="$pkg_cfg --with-included-glib"
pkg_cfg="$pkg_cfg --with-included-libunistring"
pkg_cfg="$pkg_cfg --with-included-libcroco"
pkg_cfg="$pkg_cfg --with-libiconv-prefix=$ext_dir/build/iconv/$os_name"
pkg_cfg="$pkg_cfg --with-libxml2-prefix=$ext_dir/build/xml2/$os_name"

if [ "$is_osx" -eq 1 ]
then
    pkg_cfg="$pkg_cfg --disable-openmp"

fi

####################################################################################################
# Boot pkg build scripts
####################################################################################################

function boot_gettext()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    push_dir "$ext_dir/pkgs/$pkg_base"
    separator

    # Patch the autoconf script for OSX to avoid stpncpy macro clash 
    if [ "$is_osx" -eq 1 ]
    then
        report "Applying OSX patch for autoconf script ... "
        separator
        patch -p1 < $scs_dir/data/gettext/gettext-0.18.1.1_osx_stpncpy_full.patch
        separator
    fi
    
    pop_dir

}

####################################################################################################
# build and install pkg into external folder
####################################################################################################

function build_gettext()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_opt=$5
    local pkg_cflags=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

    local existing=$(has_pkg "$pkg_name" "$pkg_opt")

    if [ $existing != 0 ]
    then
        separator
        report "Skipping existing package '$pkg_name' ... "
        return
    fi

    separator
    echo "PkgName:      '$pkg_name'"
    echo "PkgBase:      '$pkg_base'"
    echo "PkgFile:      '$pkg_file'"
    echo "PkgUrl:       '$pkg_url'"
    echo "PkgOpt:       '$pkg_opt'"
    echo "PkgCFlags:    '$pkg_cflags'"
    echo "PkgLDFlags:   '$pkg_ldflags'"
    echo "PkgCFG:       '$pkg_cfg'"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    boot_gettext   $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg 
    make_pkg    $pkg_name $pkg_base $pkg_file $pkg_url 
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_gettext $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

