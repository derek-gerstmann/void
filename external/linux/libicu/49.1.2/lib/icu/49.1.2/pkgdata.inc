GENCCODE_ASSEMBLY_TYPE=-a gcc
SO=so
SOBJ=so
A=a
LIBPREFIX=lib
LIB_EXT_ORDER=.49.1.2
COMPILE=gcc -I/opt/bldr/2012-10-05/local/compression/zlib/default/include -I/usr/include -fPIC -D_REENTRANT   -DU_ATTRIBUTE_DEPRECATED= -I/opt/bldr/2012-10-05/local/compression/zlib/default/include -I/usr/include -fPIC -Wall -ansi -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -c
LIBFLAGS=-I/opt/bldr/2012-10-05/local/text/libicu/49.1.2/include -DPIC -fPIC
GENLIB=gcc -I/opt/bldr/2012-10-05/local/compression/zlib/default/include -I/usr/include -fPIC -Wall -ansi -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long  -L/opt/bldr/2012-10-05/local/compression/zlib/default/lib -L/usr/lib64 -L/usr/lib -fPIC  -shared -Wl,-Bsymbolic
LDICUDTFLAGS=-nodefaultlibs -nostdlib
LD_SONAME=-Wl,-soname -Wl,
RPATH_FLAGS=
BIR_LDFLAGS=-Wl,-Bsymbolic
AR=ar
ARFLAGS=r
RANLIB=ranlib
INSTALL_CMD=/opt/bldr/2012-10-05/local/internal/coreutils/8.17/bin/install -c
