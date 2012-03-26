#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg dependency paths
####################################################################################################

ilm_base="$ext_dir/build/exr/$os_name"
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
mpi_base="$ext_dir/mpi"

mpi_libs="$mpi_base/lib/$os_name/libmpi.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libmpi_cxx.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libmpi_f77.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libmpi_f90.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libompitrace.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libopen-rte.a"
mpi_libs="$mpi_libs\;$mpi_base/lib/$os_name/libopen-pal.a"

hdf_libs="$hdf_base/lib/libhdf5.a"
hdf_libs="$hdf_libs\;$hdf_base/lib/libhdf5_hl.a"
hdf_libs="$hdf_libs\;$hdf_base/lib/libhdf5_cpp.a"
hdf_libs="$hdf_libs\;$zlib_base/lib/libz.a"
hdf_libs="$hdf_libs\;$szip_base/lib/libsz.a"
hdf_libs="$hdf_libs\;$mpi_libs"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################

pkg_name="f3d"
pkg_base="imageworks-Field3D-94d1bf3"
pkg_file="imageworks-Field3D-v1.3.2-0-gcac7b57.tar.gz"
pkg_url="http://github.com/imageworks/Field3D/tarball/v1.3.2"

pkg_mpath="$zlib_base"
pkg_mpath="$pkg_mpath:$ilm_base"
pkg_mpath="$pkg_mpath:$exr_base"
pkg_mpath="$pkg_mpath:$zlib_base"
pkg_mpath="$pkg_mpath:$szip_base"
pkg_mpath="$pkg_mpath:$hdf_base"
pkg_mpath="$pkg_mpath:$tiff_base"
pkg_mpath="$pkg_mpath:$png_base"
pkg_mpath="$pkg_mpath:$jpeg_base"
pkg_mpath="$pkg_mpath:$jp2k_base"
pkg_mpath="$pkg_mpath:$glew_base"
pkg_mpath="$pkg_mpath:$mpi_base"
pkg_mpath="$pkg_mpath:$boost_base"

pkg_env="-DMAKESTATIC=1:-DLINKSTATIC=1"
pkg_env="$pkg_env:-DCMAKE_C_FLAGS=-I$mpi_base/include"
pkg_env="$pkg_env:-DCMAKE_CXX_FLAGS=-I$mpi_base/include"

pkg_env="$pkg_env:-DIlmbase_Base_Dir=$ilm_base"
pkg_env="$pkg_env:-DILMBASE_INCLUDE_DIR=$ilm_base/include"
pkg_env="$pkg_env:-DILMBASE_HALF_LIBRARIES=$ilm_base/lib/libHalf.a"
pkg_env="$pkg_env:-DILMBASE_IEX_LIBRARIES=$ilm_base/lib/libIex.a"
pkg_env="$pkg_env:-DILMBASE_IMATH_LIBRARIES=$ilm_base/lib/libImath.a"
pkg_env="$pkg_env:-DILMBASE_ILMTHREAD_LIBRARIES=$ilm_base/lib/libIlmThread.a"

pkg_env="$pkg_env:-DBoost_DIR=$boost_base"
pkg_env="$pkg_env:-DBoost_INCLUDE_DIR=$boost_base/include"
pkg_env="$pkg_env:-DBoost_LIBRARY_DIRS=$boost_base/lib"
pkg_env="$pkg_env:-DBoost_PROGRAM_OPTIONS_LIBRARY=$boost_base/lib/libboost_program_options.a"
pkg_env="$pkg_env:-DBoost_THREAD_LIBRARY=$boost_base/lib/libboost_thread.a"

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

pkg_cfg="-DCMAKE_PREFIX_PATH=$ext_dir"
pkg_cfg="$pkg_cfg -DCMAKE_EXE_LINKER_FLAGS='-L$mpi_base/lib/$os_name -lmpi -lmpi_cxx -lompitrace -lopen-rte -lopen-pal' "
pkg_cfg="$pkg_cfg -DHDF5_INCLUDE_DIR=$hdf_base/include"
pkg_cfg="$pkg_cfg -DHDF5_LIBRARIES='$hdf_libs'"
pkg_cfg="$pkg_cfg -DZLIB_INCLUDE_DIR=$zlib_base/include"
pkg_cfg="$pkg_cfg -DZLIB_LIBRARY=$zlib_base/lib/$os_name/libz.a"
pkg_cfg="$pkg_cfg -DSZIP_INCLUDE_DIR=$szip_base/include"
pkg_cfg="$pkg_cfg -DSZIP_LIBRARY=$szip_base/lib/$os_name/libsz.a"

pkg_opt="cmake:keep"

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_mpath $pkg_env $pkg_cfg


