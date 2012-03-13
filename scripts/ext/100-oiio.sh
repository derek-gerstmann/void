#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################

function cmake_oiio()
{
    # configure package
    local m=7
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_cmkpaths=$5
    local pkg_envflags=$6
    local pkg_cfg="${@:$m}"

    cd "$pkg_base"
    mkdir -p build
    cd build

    separator
  
    prefix="$ext_dir/build/$pkg_name/$os_name"
    env_flags=" "
    if [ -n $pkg_cmkpaths ] && [ $pkg_cmkpaths != 0 ]
    then
        pkg_cmkpaths=$(echo $pkg_cmkpaths | split_str ":" | join_str " ")
    	env_flags='-DCMAKE_MODULE_PATH="'$pkg_cmkpaths'"'
    fi
        
    if [ -n $pkg_envflags ] && [ $pkg_envflags != 0 ]
    then
        pkg_envflags=$(echo $pkg_envflags | split_str ":" | join_str " ")
    	env_flags=$env_flags' '$pkg_envflags
    fi

    report "Configuring package '$pkg_name' ..."
    separator
    echo cmake -DCMAKE_INSTALL_PREFIX="$prefix" $env_flags ../src
    separator
    eval cmake -DCMAKE_INSTALL_PREFIX="$prefix" $env_flags ../src || bail "Failed to configure: '$prefix'"
    separator
    report "Done configuring package '$pkg_name'"
}


function build_oiio()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5
    local pkg_cmkpaths=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

#    echo "PkgName: '$pkg_name' PkgBase: '$pkg_base' PkgFile: '$pkg_file' PkgUrl: '$pkg_url' PkgCfg: '$pkg_cfg' PkgKeep: '$pkg_keep'"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    cmake_oiio  $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cmkpaths $pkg_ldflags $pkg_cfg
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################

# setup pkg definition and resource files
pkg_name="oiio"
pkg_base="OpenImageIO-oiio-cc5028d"
pkg_file="OpenImageIO-oiio-Release-1.0.0-19-gcc5028d.tar.gz"
pkg_url="http://nodeload.github.com/OpenImageIO/oiio/tarball/RB-1.0"
pkg_cfg="--disable-shared --enable-static"

ilm_base="$ext_dir/exr"
exr_base="$ext_dir/exr"
zlib_base="$ext_dir/zlib"
szip_base="$ext_dir/szip"
hdf_base="$ext_dir/hdf"
tiff_base="$ext_dir/tiff"
f3d_base="$ext_dir/f3d"
png_base="$ext_dir/png"
jpeg_base="$ext_dir/jpeg"
jp2k_base="$ext_dir/jp2k"
glew_base="$ext_dir/glew"

pkg_cmkpaths="$ext_dir/build/tiff/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ilm_base"
pkg_cmkpaths="$pkg_cmkpaths:$exr_base"
pkg_cmkpaths="$pkg_cmkpaths:$zlib_base"
pkg_cmkpaths="$pkg_cmkpaths:$hdf_base"
pkg_cmkpaths="$pkg_cmkpaths:$f3d_base"
pkg_cmkpaths="$pkg_cmkpaths:$png_base"
pkg_cmkpaths="$pkg_cmkpaths:$jpeg_base"
pkg_cmkpaths="$pkg_cmkpaths:$jp2k_base"

pkg_envflags="-DMAKESTATIC=1:-DLINKSTATIC=1"

pkg_envflags="$pkg_envflags:-DILMBASE_HOME=$ilm_base"
pkg_envflags="$pkg_envflags:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_envflags="$pkg_envflags:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/$os_name/libHalf.a"
pkg_envflags="$pkg_envflags:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/$os_name/libIex.a"
pkg_envflags="$pkg_envflags:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/$os_name/libImath.a"
pkg_envflags="$pkg_envflags:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/$os_name/libIlmThread.a"

pkg_envflags="$pkg_envflags:-DOPENEXR_HOME=$ilm_base"
pkg_envflags="$pkg_envflags:-DOPENEXR_INCLUDE_DIR=$ilm_base/include/OpenEXR"
pkg_envflags="$pkg_envflags:-DOPENEXR_ILMIMF_LIBRARIES=$ilm_base/lib/$os_name/libIlmImf.a"

pkg_envflags="$pkg_envflags:-DHDF5_HOME=$hdf_base"
pkg_envflags="$pkg_envflags:-DHDF5_LIBRARY_DIRS=$hdf_base/lib"
pkg_envflags="$pkg_envflags:-DHDF5_INCLUDE_DIRS=$hdf_base/include"
pkg_envflags="$pkg_envflags:-DHDF5_LIBRARIES=$hdf_base/lib/$os_name/libhdf5.a\;$hdf_base/lib/$os_name/libhdf5_hl.a\;$zlib_base/lib/$os_name/libz.a\;$szip_base/lib/$os_name/libsz.a"

pkg_envflags="$pkg_envflags:-DFIELD3D_HOME=$f3d_base"
pkg_envflags="$pkg_envflags:-DFIELD3D_INCLUDE_DIR=$f3d_base/include/Field3D"
pkg_envflags="$pkg_envflags:-DFIELD3D_LIBRARY=$f3d_base/lib/$os_name/libField3D.a"

pkg_envflags="$pkg_envflags:-DGLEW_INCLUDE_DIR=$glew_base/include"
pkg_envflags="$pkg_envflags:-DGLEW_LIBRARIES=$glew_base/lib/$os_name/libGLEW.a"

pkg_envflags="$pkg_envflags:-DJPEG_INCLUDE_DIR=$jpeg_base/include"
pkg_envflags="$pkg_envflags:-DJPEG_LIBRARY=$jpeg_base/lib/$os_name/libjpeg.a"

pkg_envflags="$pkg_envflags:-DOPENJPEG_INCLUDE_DIR=$jp2k_base/include"
pkg_envflags="$pkg_envflags:-DOPENJPEG_OPENJPEG_LIBRARIES=$jp2k_base/lib/$os_name/libopenjpeg.a"
pkg_envflags="$pkg_envflags:-DOPENJPEG_LIBRARY=$jp2k_base/lib/$os_name/libopenjpeg.a"

pkg_envflags="$pkg_envflags:-DTIFF_INCLUDE_DIR=$tiff_base/include"
pkg_envflags="$pkg_envflags:-DTIFF_LIBRARY=$tiff_base/lib/$os_name/libtiff.a"

pkg_envflags="$pkg_envflags:-DPNG_PNG_INCLUDE_DIR=$png_base/include"
pkg_envflags="$pkg_envflags:-DPNG_LIBRARY=$png_base/lib/$os_name/libpng.a"

pkg_envflags="$pkg_envflags:-DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_envflags="$pkg_envflags:-DZLIB_LIBRARY=$zlib_base/lib/$os_name/libz.a"

pkg_keep=1

# build and install pkg into external folder
build_oiio $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cmkpaths $pkg_envflags $pkg_cfg


