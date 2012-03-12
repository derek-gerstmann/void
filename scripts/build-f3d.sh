#!/bin/bash

####################################################################################################

function cmake_f3d()
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
    echo cmake -DCMAKE_INSTALL_PREFIX="$prefix" $env_flags ..
    separator
    eval cmake -DCMAKE_INSTALL_PREFIX="$prefix" $env_flags .. || bail "Failed to configure: '$prefix'"
    separator
    report "Done configuring package '$pkg_name'"
}

function build_f3d()
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
    cmake_f3d   $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cmkpaths $pkg_ldflags $pkg_cfg
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################

# source common configuration settings
source "./common.sh"
source "./build-pkg.sh"

# setup pkg definition and resource files
pkg_name="f3d"
pkg_base="imageworks-Field3D-94d1bf3"
pkg_file="imageworks-Field3D-v1.3.2-0-gcac7b57.tar.gz"
pkg_url="http://github.com/imageworks/Field3D/tarball/v1.3.2"
pkg_cfg="--disable-shared --enable-static"
pkg_keep=1

pkg_cmkpaths="$ext_dir/build/tiff/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/zlib/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/png/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/jpeg/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/jp2k/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/hdf/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/glew/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/ilm/$os_name"
pkg_cmkpaths="$pkg_cmkpaths:$ext_dir/build/exr/$os_name"

ilm_base="$ext_dir/build/exr/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"

pkg_envflags="MAKESTATIC=1:LINKSTATIC=1"

pkg_envflags="$pkg_envflags:-DIlmbase_Base_Dir=$ilm_base"
pkg_envflags="$pkg_envflags:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_envflags="$pkg_envflags:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/libHalf.a"
pkg_envflags="$pkg_envflags:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/libIex.a"
pkg_envflags="$pkg_envflags:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/libImath.a"
pkg_envflags="$pkg_envflags:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/libIlmThread.a"

pkg_envflags="$pkg_envflags:-DHDF5_HOME=$hdf_base"
pkg_envflags="$pkg_envflags:-DHDF5_CXX_COMPILER_EXECUTABLE=$hdf_base/bin/h5c++"
pkg_envflags="$pkg_envflags:-DHDF5_C_COMPILER_EXECUTABLE=$hdf_base/bin/h5cc"
pkg_envflags="$pkg_envflags:-DHDF5_C_INCLUDE_DIR=$hdf_base/include"
pkg_envflags="$pkg_envflags:-DHDF5_LIBRARIES=$hdf_base/lib/libhdf5.a"
pkg_envflags="$pkg_envflags:-DHDF5_hdf5_LIBRARY=$hdf_base/lib/libhdf5.a"
pkg_envflags="$pkg_envflags:-DHDF5_hdf5_hl_LIBRARY=$hdf_base/lib/libhdf5_hl.a"
pkg_envflags="$pkg_envflags:-DHDF5_z_LIBRARY=$zlib_base/lib/libz.a"

pkg_keep=1

# build and install pkg into external folder
build_f3d $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep $pkg_cmkpaths $pkg_envflags $pkg_cfg


