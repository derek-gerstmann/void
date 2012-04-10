#!/bin/bash

# source common configuration settings
source "./common.sh"

####################################################################################################
# setup pkg definition and resource files
####################################################################################################
local pkg_cli="$1"
if [ -n $pkg_cli ]
then
	pkg_cli="mvapich2"
fi

if [[ $(echo $pkg_cli | grep -c 'openmpi' ) > 0 ]]
then
	pkg_name="mpi"
	pkg_base="openmpi-1.5.4"
	pkg_file="$pkg_base.tar.bz2"
	pkg_url="http://www.open-mpi.org/software/ompi/v1.5/downloads/$pkg_file"

	pkg_opt="configure:keep"
	pkg_cflags="-I$ext_dir/zlib/include"
	pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
	pkg_cfg="--disable-shared --enable-static --enable-btl-openib-failover --enable-heterogeneous --enable-mpi-thread-multiple"

	if [ "$is_osx" -eq 1 ]
	then
    	# Building v1.5.4 on OSX causes error due to missing '__builtin_expect' -- disable vampire trace avoids this
	    pkg_cfg="$pkg_cfg --disable-vt "
	fi

elif [[ $(echo $pkg_cli | grep -c 'mvapich2' ) > 0 ]]
then

	pkg_name="mpi"
	pkg_base="mvapich2-1.8rc1"
	pkg_file="mvapich2-1.8rc1.tgz"
	pkg_url="http://mvapich.cse.ohio-state.edu/download/mvapich2/mvapich2-1.8rc1.tgz"

	pkg_opt="configure:keep"
	pkg_cflags="-I$ext_dir/zlib/include"
	pkg_ldflags="-L$ext_dir/zlib/lib/$os_name"
	pkg_cfg="--disable-shared --enable-static"
	if [ "$is_osx" -eq 0 ]
	then
		pkg_cfg="$pkg_cfg --with-cuda=/usr/local/cuda"
		pkg_cfg="$pkg_cfg --with-cuda-include=/usr/local/cuda/include"
		pkg_cfg="$pkg_cfg --with-cuda-libpath=/usr/local/cuda/lib64"
	fi
	pkg_cfg="$pkg_cfg --enable-threads=runtime"
	pkg_cfg="$pkg_cfg --with-thread-package=posix"
	pkg_cfg="$pkg_cfg --enable-romio"
	pkg_cfg="$pkg_cfg --enable-cxx"

	if [ "$is_osx" -eq 1 ]
	then
    	# Building v1.5.4 on OSX causes error due to missing '__builtin_expect' -- disable vampire trace avoids this
	    pkg_cfg="$pkg_cfg --disable-vt "
	fi

else
	bail "Unknown MPI implementation specificied: '$pkg_cli'!"
fi

####################################################################################################
# build and install pkg into external folder
####################################################################################################

build_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_opt $pkg_cflags $pkg_ldflags $pkg_cfg

