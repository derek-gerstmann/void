GENCCODE_ASSEMBLY_TYPE=-a gcc-darwin
SO=dylib
SOBJ=dylib
A=a
LIBPREFIX=lib
LIB_EXT_ORDER=.49.1.dylib
COMPILE=gcc -DU_ATTRIBUTE_DEPRECATED= -I/Users/dg/Workspace/void/clean/void/external/zlib/include    -I/Users/dg/Workspace/void/clean/void/external/zlib/include -Wall -ansi -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -fno-common -c
LIBFLAGS=-I/Users/dg/Workspace/void/clean/void/external/build/icu/osx/include  -dynamic
GENLIB=gcc -dynamiclib -dynamic -I/Users/dg/Workspace/void/clean/void/external/zlib/include -Wall -ansi -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -L/Users/dg/Workspace/void/clean/void/external/zlib/lib/osx  
LDICUDTFLAGS=
LD_SONAME=-Wl,-compatibility_version -Wl,49 -Wl,-current_version -Wl,49.1 -install_name
RPATH_FLAGS=
BIR_LDFLAGS=
AR=ar
ARFLAGS=r
RANLIB=ranlib
INSTALL_CMD=/usr/bin/install -c
