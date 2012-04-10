#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="oiio"
pkg_base="OpenImageIO-oiio-388f38b"
pkg_file="$pkg_base.zip"
pkg_url="http://github.com/OpenImageIO/oiio/zipball/RB-1.0"
pkg_opt="cmake:keep"

####################################################################################################
# setup pkg dependency paths
####################################################################################################

ilm_base="$ext_dir/build/ilm/$os_name"
exr_base="$ext_dir/build/exr/$os_name"
zlib_base="$ext_dir/build/zlib/$os_name"
szip_base="$ext_dir/build/szip/$os_name"
hdf_base="$ext_dir/build/hdf/$os_name"
tiff_base="$ext_dir/build/tiff/$os_name"
f3d_base="$ext_dir/build/f3d/$os_name"
png_base="$ext_dir/build/png/$os_name"
jpeg_base="$ext_dir/build/jpeg/$os_name"
jp2k_base="$ext_dir/build/jp2k/$os_name"
glew_base="$ext_dir/build/glew/$os_name"
boost_base="$ext_dir/build/boost/$os_name"
mpi_base="$ext_dir/build/mpi/$os_name"

mpi_link="-L$mpi_base/lib"
mpi_link="$mpi_link $mpi_base/lib/libmpi.a"
mpi_link="$mpi_link $mpi_base/lib/libmpi_cxx.a"
mpi_link="$mpi_link $mpi_base/lib/libompitrace.a"
mpi_link="$mpi_link $mpi_base/lib/libopen-rte.a"
mpi_link="$mpi_link $mpi_base/lib/libopen-pal.a"

if [ "$is_centos" -eq 1 ]
then
	mpi_link="$mpi_link -L/usr/lib64 -L/usr/lib -lpthread $zlib_base/lib/libz.a $ilm_base/lib/libIlmThread.a"
else
	mpi_link="$mpi_link -lpthread"
fi

mpi_libs="$mpi_base/lib/libmpi.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libmpi_cxx.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libmpi_f77.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libmpi_f90.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libompitrace.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libopen-rte.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/libopen-pal.a"

hdf_link="-L$hdf_base/lib "
hdf_link="$hdf_link $hdf_base/lib/libhdf5.a"
hdf_link="$hdf_link $hdf_base/lib/libhdf5_hl.a"
hdf_link="$hdf_link $hdf_base/lib/libhdf5_cpp.a"
hdf_link="$hdf_link $zlib_base/lib/libz.a"
hdf_link="$hdf_link $szip_base/lib/libsz.a"

hdf_libs="$hdf_base/lib/libhdf5.a"
hdf_libs="$hdf_libs\;$hdf_base/lib/libhdf5_hl.a"
hdf_libs="$hdf_libs\;$hdf_base/lib/libhdf5_cpp.a"
hdf_libs="$hdf_libs\;$zlib_base/lib/libz.a"
hdf_libs="$hdf_libs\;$szip_base/lib/libsz.a"
hdf_libs="$hdf_libs\;$mpi_libs"

ilm_libs="ilm_base/lib/libHalf.a"
ilm_libs="$ilm_libs\;$ilm_base/lib/libIex.a"
ilm_libs="$ilm_libs\;$ilm_base/lib/libIexMath.a"
ilm_libs="$ilm_libs\;$ilm_base/lib/libImath.a"
ilm_libs="$ilm_libs\;$ilm_base/lib/libIlmThread.a"
ilm_libs="$ilm_libs\;$zlib_base/lib/libz.a"
ilm_libs="$ilm_libs\;-lpthread_nonshared"

exr_libs="$ilm_base/lib/libHalf.a"
exr_libs="$exr_libs $ilm_base/lib/libIex.a"
exr_libs="$exr_libs $ilm_base/lib/libIexMath.a"
exr_libs="$exr_libs $ilm_base/lib/libImath.a"
exr_libs="$exr_libs $ilm_base/lib/libIlmThread.a"
exr_libs="$exr_libs $exr_base/lib/libIlmImf.a"
exr_libs="$exr_libs $zlib_base/lib/libz.a"
exr_libs="$exr_libs -lpthread"
exr_link="-L$ilm_base/lib $exr_libs"

pkg_mpath="$ext_dir:$zlib_base:$png_base:$jpeg_base:$boost_base:$exr_base:$hdf_base:$mpi_base"

pkg_env="-DBUILDSTATIC=ON"
pkg_env="$pkg_env:-DLINKSTATIC=ON"
pkg_env="$pkg_env:-DEMBEDPLUGINS=ON"
pkg_env="$pkg_env:-DUSE_OPENGL=ON"
pkg_env="$pkg_env:-DUSE_PYTHON=OFF"
pkg_env="$pkg_env:-DUSE_FIELD3D=ON"
pkg_env="$pkg_env:-DUSE_OPENJPEG=ON"
pkg_env="$pkg_env:-DUSE_QT=OFF"
pkg_env="$pkg_env:-DBUILD_TESTING=OFF"

pkg_env="$pkg_env:-DILMBASE_HOME=$ilm_base"
pkg_env="$pkg_env:-DILMBASE_VERSION=1.0.3"
pkg_env="$pkg_env:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_env="$pkg_env:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/libHalf.a"
pkg_env="$pkg_env:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/libIex.a\;$ilm_base/lib/libIexMath.a"
pkg_env="$pkg_env:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/libImath.a"
pkg_env="$pkg_env:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/libIlmThread.a"
pkg_env="$pkg_env:-DILMBASE_LIBRARIES=$ilm_libs"

pkg_env="$pkg_env:-DOPENEXR_HOME=$ilm_base"
pkg_env="$pkg_env:-DOPENEXR_INCLUDE_DIR=$ilm_base/include/OpenEXR"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARIES=$exr_base/lib/libIlmImf.a\;$ilm_libs"
pkg_env="$pkg_env:-DOPENEXR_ILMIMF_LIBRARY=$exr_base/lib/libIlmImf.a"

pkg_env="$pkg_env:-DHDF5_DIR=$hdf_base"
pkg_env="$pkg_env:-DHDF5_INCLUDE_DIRS=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_LIBRARIES=$hdf_libs"
pkg_env="$pkg_env:-DHDF5_CXX_COMPILER_EXECUTABLE=$hdf_base/bin/h5c++"
pkg_env="$pkg_env:-DHDF5_C_COMPILER_EXECUTABLE=$hdf_base/bin/h5cc"
pkg_env="$pkg_env:-DHDF5_C_INCLUDE_DIR=$hdf_base/include"
pkg_env="$pkg_env:-DHDF5_hdf5_LIBRARY_RELEASE=$hdf_base/lib/libhdf5.a"
pkg_env="$pkg_env:-DHDF5_hdf5_hl_LIBRARY_RELEASE=$hdf_base/lib/libhdf5_hl.a"
pkg_env="$pkg_env:-DHDF5_hdf5_cpp_LIBRARY_RELEASE=$hdf_base/lib/libhdf5_cpp.a"
pkg_env="$pkg_env:-DHDF5_z_LIBRARY_RELEASE=$zlib_base/lib/libz.a"
pkg_env="$pkg_env:-DHDF5_sz_LIBRARY_RELEASE=$szip_base/lib/libsz.a"

pkg_env="$pkg_env:-DFIELD3D_HOME=$f3d_base"
pkg_env="$pkg_env:-DFIELD3D_INCLUDE_DIR=$f3d_base/include/Field3D"
pkg_env="$pkg_env:-DFIELD3D_LIBRARY=$f3d_base/lib/libField3D.a"

pkg_env="$pkg_env:-DGLEW_INCLUDE_DIR=$glew_base/include"
pkg_env="$pkg_env:-DGLEW_INCLUDES=$glew_base/include"
pkg_env="$pkg_env:-DGLEW_LIBRARIES=$glew_base/lib/libGLEW.a"

pkg_env="$pkg_env:-DJPEG_INCLUDE_DIR=$jpeg_base/include"
pkg_env="$pkg_env:-DJPEG_LIBRARY=$jpeg_base/lib/libjpeg.a"

pkg_env="$pkg_env:-DOPENJPEG_INCLUDE_DIR=$jp2k_base/include"
pkg_env="$pkg_env:-DOPENJPEG_OPENJPEG_LIBRARIES=$jp2k_base/lib/libopenjpeg.a"
pkg_env="$pkg_env:-DOPENJPEG_OPENJPEG_LIBRARY=$jp2k_base/lib/libopenjpeg.a"
pkg_env="$pkg_env:-DOPENJPEG_LIBRARY=$jp2k_base/lib/libopenjpeg.a"

pkg_env="$pkg_env:-DTIFF_INCLUDE_DIR=$tiff_base/include"
pkg_env="$pkg_env:-DTIFF_LIBRARY=$tiff_base/lib/libtiff.a"

pkg_env="$pkg_env:-DPNG_PNG_INCLUDE_DIR=$png_base/include"
pkg_env="$pkg_env:-DPNG_LIBRARY=$png_base/lib/libpng.a"

pkg_env="$pkg_env:-DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_env="$pkg_env:-DZLIB_LIBRARY=$zlib_base/lib/libz.a"

pkg_env="$pkg_env:-DBoost_INCLUDE_DIR=$boost_base/include"
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib"
pkg_env="$pkg_env:-DBoost_FILESYSTEM_LIBRARY=$boost_base/lib/libboost_filesystem.a"
pkg_env="$pkg_env:-DBoost_PYTHON_LIBRARY=$boost_base/lib/libboost_python.a"
pkg_env="$pkg_env:-DBoost_REGEX_LIBRARY=$boost_base/lib/libboost_regex.a"
pkg_env="$pkg_env:-DBoost_SYSTEM_LIBRARY=$boost_base/lib/libboost_system.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/libboost_thread.a"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/libboost_program_options.a"
    
pkg_cfg="-DCMAKE_EXE_LINKER_FLAGS='$mpi_link $exr_link'"
# pkg_cfg="$pkg_cfg -DCMAKE_MODULE_LINKER_FLAGS='$mpi_link $hdf_link $exr_link'"
# pkg_cfg="$pkg_cfg -DCMAKE_C_FLAGS='-I$mpi_base/include -I$boost_base/include'"
# pkg_cfg="$pkg_cfg -DCMAKE_CXX_FLAGS='-I$mpi_base/include -I$boost_base/include -L$boost_base/lib'"
pkg_cfg="$pkg_cfg -DHDF5_INCLUDE_DIRS=$hdf_base/include"
pkg_cfg="$pkg_cfg -DHDF5_INCLUDE_DIR=$hdf_base/include"
pkg_cfg="$pkg_cfg -DHDF5_CXX_INCLUDE_DIR=$hdf_base/include"
#pkg_cfg="$pkg_cfg -DHDF5_LIBRARIES='$hdf_libs'"
#pkg_cfg="$pkg_cfg -DHDF5_CXX_LIBRARIES='$hdf_libs'"
pkg_cfg="$pkg_cfg -DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_cfg="$pkg_cfg -DZLIB_LIBRARY=$zlib_base/lib/libz.a"
pkg_cfg="$pkg_cfg -DILMBASE_HOME=$ilm_base"
pkg_cfg="$pkg_cfg -DOPENEXR_INCLUDE_DIR=$exr_base/include"
pkg_cfg="$pkg_cfg -DFIELD3D_HOME=$f3d_base"
pkg_cfg="$pkg_cfg -DBOOST_ROOT=$boost_base"

# Geez, Sony calls CMAKE inside their CMakeLists.txt file so we need to prepend our version to the path
export PATH=$ext_dir/cmake/bin/$os_name:$PATH


####################################################################################################
# Boot pkg build scripts
####################################################################################################

function boot_oiio()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    push_dir "$ext_dir/pkgs/$pkg_base"
    separator

    # Disable the horribly broken imagebufalgo_test from the OIIO CMakeLists
    sed -i "s/\(.*\)\(imagebufalgo_test\)/\#\1\2/g" src/libOpenImageIO/CMakeLists.txt

    # Disable all tool subdirectories
    sed -i "s/\(.*\)\(add_subdirectory (iv)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (iconvert)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (idiff)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (igrep)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (iinfo)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (iprocess)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (testtex)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (oiiotool)\)/\# \1\2/g" src/CMakeLists.txt
    sed -i "s/\(.*\)\(add_subdirectory (maketx)\)/\# \1\2/g" src/CMakeLists.txt
    pop_dir

}

####################################################################################################
# build and install pkg into external folder
####################################################################################################

function build_oiio()
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
    boot_oiio   $pkg_name $pkg_base $pkg_file $pkg_url
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

build_oiio $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_mpath $pkg_env $pkg_cfg


