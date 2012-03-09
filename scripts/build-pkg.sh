#!/bin/bash

####################################################################################################

export ABSPATH=$(cd ${0%/*} && echo $PWD)
export ID=$( basename "$ABSPATH" )

####################################################################################################

function lowercase()
{
    echo $1 | tr '[:upper:]' '[:lower:]'
}

function uppercase()
{
    echo $1 | tr '[:lower:]' '[:upper:]'
}

function resolve()
{
        cd "$1" 2>/dev/null || return $? 
        local resolved="`pwd -P`"
        echo "$resolved"
}

function abswd
{
    if [[ $(echo $0 | awk '/^\//') == $0 ]]; then
        export ABSPATH=$(dirname $0)
    else
        export ABSPATH=$PWD/$(dirname $0)
    fi
}

function report()
{
    local ts=$(date "+%Y-%m-%d-%H:%M:%S")
    echo "[ ${ts} ] ${1}"
}

function separator()
{
    report "---------------------------------------------------------------------------------------"
}

function header()
{
    local msg=$@
    separator
    report "${msg}"
    separator
}

function bail
{
    local msg=$@

    report ""

    separator
    report "ERROR: ${msg}. Exiting..."
    separator

    exit 1
}

function try
{
    $@
    if test $? -ne 0 ; then
        bail "$@"
    fi
}

function exit_on_failure
{
    if test $? -ne 0 ; then
        bail "$1"
    fi
}

function clone()
{
    local url=$1
    local dir=$2

    local cmd="$(which wget)"
    if [ -e $cmd ];
    then
        cmd="git clone"
        $cmd $url $dir || bail "Failed to clone '$url' into '$dir'"
    else
        bail "Failed to locate 'git' command!  Please install this command line utility!"
    fi
    separator
}

function fetch() 
{
    local url=$1
    local tarball=$2
    local dir=$3

    local usepipe=0
    local cmd="$(which wget)"
    if [ ! -e $cmd ];
    then
        cmd="$(which curl)"
        if [ -e $cmd ];
        then
            cmd="curl -f -L -O"
            usepipe=1
        else
            bail "Failed to locate either 'wget' or 'curl' for fetching packages!  Please install one of these utilties!"
        fi
    else
        cmd="wget --progress=dot -O"
    fi

    if [ ! -e $tarball ]; then
        report "Fetching '$tarball'..."
        separator
        if [ "$usepipe" -eq 1 ]
        then
            echo "$cmd $url > $tarball"
			$cmd $url > $tarball
		else
            echo "$cmd $tarball $url"
	        $cmd $tarball $url
	    fi
        separator
        report "Downloaded '$url' to '$tarball'..."
    fi

    if [ ! -e $tarball ]; then
        bail "Failed to fetch '$tarball'..."
    fi
}

function extract_tarball()
{
	local tarball=$1
    if test -f ${tarball%.*}*.bz2 ; then
            tar jxf ${tarball} || bail "Failed to extract tarball '${tarball}'"
    elif  test -f ${tarball%.*}*.gz ; then
            tar zxf ${tarball} || bail "Failed to extract tarball '${tarball}'"
    elif  test -f ${tarball%.*}*.tgz ; then
            tar zxf ${tarball} || bail "Failed to extract tarball '${tarball}'"
    elif  test -f ${tarball%.*}*.zip ; then
            unzip -uo ${tarball} || bail "Failed to extract tarball '${tarball}'"
    fi	
}

function make_dir()
{
    local dir=$1
    mkdir -p ${dir} >${LOG_FILE} || bail "Failed to create directory '${dir}'"
}

function remove_dir()
{
    local dir=$1
    local force=$2
    if [ ${force} ]; then
        rm -rf ${dir} >${LOG_FILE} || bail "Failed to remove directory '${dir}'"
    else
        if [ -d "${dir}" ]; then
            rm -r ${dir} >${LOG_FILE} || bail "Failed to remove directory '${dir}'"
        fi
    fi
}

function push_dir()
{
    local dir=$1
    pushd ${dir} >${LOG_FILE} || bail "Failed to push directory '${dir}'"
}

function pop_dir()
{
    local dir=$1
    popd >${LOG_FILE} || bail "Failed to pop directory!"
}

function move_file()
{
    local src=$1
    local dst=$2
    mv ${src} ${dst} >${LOG_FILE} || bail "Failed to move directory!"
}

function make_tarball()
{
    local tarball=$1
    local dir=$2
    
    tar cjf "${tarball}" "${dir}" || bail "Failed to create tarball!"
}

function get_cursor_position()
{
    exec < /dev/tty
    oldstty=$(stty -g)
    stty raw -echo min 0
    echo -en "\033[6n" > /dev/tty
    # tput u7 > /dev/tty    # when TERM=xterm (and relatives)
    
    IFS=';' read -r -d R -a pos
    stty $oldstty
    
    # change from one-based to zero based so they work with: tput cup $row $col
    local row=$((${pos[0]:2} - 1))    # strip off the esc-[
    local col=$((${pos[1]} - 1))
    
    echo "$row $col"
}

####################################################################################################

# setup project paths
abscwd="$( cd "$( dirname "$0" )" && pwd )"
basedir="$( basename "$abscwd" )"
rootdir="$( dirname "$abscwd" )"
extdir="$rootdir/external"
osname="unknown"
islnx=$( uname -s | grep -c Linux )
if [ "$islnx" -eq 1 ]
then
	osname="lnx"
fi

isosx=$( uname -s | grep -c Darwin )
if [ "$isosx" -eq 1 ]
then
	osname="osx"
fi

if [ "$osname" == "unknown" ]
then
	echo "Operating system is unknown and not detected properly.  Please update detection routine!"
	exit 0
fi 

# ensure we are run inside of the scripts folder
if [ "$basedir" != "scripts" ]
then
	echo "Please execute package build script from within the void/scripts subfolder!"
	exit 0
fi 

####################################################################################################

# move into external pkg folder for retrieving pkgs
prefix="$extdir/build/$pkgname/$osname"
separator
report "Setting up external package '$pkgbase' for '$osname' ... "
cd "$extdir"
mkdir -p pkgs
cd pkgs
separator

# remove any existing extracted pkg folder
if [ -d "$pkgbase" ]
then
	report "Removing old package '$pkgbase'"
	rm -r "$pkgbase"
    separator
fi

# if a local copy doesn't exist, grab the pkg from the url
if [ ! -f "$pkgfile" ]
then
	report "Retrieving package '$pkgname'"
    if [[ $(echo $pkgurl | grep -c 'http://') == 1 ]]
    then
    	fetch $pkgurl $pkgfile 
    fi
    if [[ $(echo $pkgurl | grep -c 'git://') == 1 ]]
    then
        clone $pkgurl $pkgbase 
        make_tarball $pkgfile $pkgbase
    fi
    separator
fi

# extract pkg file and move into extracted folder
if [[ $(echo $pkgurl | grep -c '.gz|.tar|.bz|.zip') == 1 ]]
then
    report "Extracting package '$pkgfile'"
    extract_tarball $pkgfile
fi

cd "$pkgbase"
separator

# bootstrap package
if [ ! -f "./configure" ]
then
    if [ -f "./bootstrap" ]
    then
        report "Bootstraping package '$pkgname'"
        separator
        ./bootstrap
        separator
    fi
    if [ -f "./bootstrap.sh" ]
    then 
        report "Bootstraping package '$pkgname'"
        separator
        ./bootstrap.sh
        separator
    fi
    if [ -f "./autogen.sh" ]
    then 
        report "Bootstraping package '$pkgname'"
        separator
        ./autogen.sh
        separator
    fi
fi

# configure package
if [ -f "./configure" ]
then
    report "Configuring package '$pkgname'"
    separator
    echo ./configure --prefix="$prefix" $pkgcfg
    separator
    ./configure --prefix="$prefix" $pkgcfg || bail "Failed to configure: '$prefix'"
    separator
    report "Done configuring package '$pkgname'"
fi

# build and install into local path
report "Building package '$pkgname'"
separator
make  || bail "Failed to build package: '$prefix'"
separator

report "Installing package '$pkgname'"
separator
make install || bail "Failed to install package: '$prefix'"
separator

# move header into external path
if [ -d "$prefix/include" ]
then
    report "Copying headers for '$pkgname'"
    separator
    mkdir -p "$extdir/$pkgname/include"
    cp -TRv  "$prefix/include" "$extdir/$pkgname/include" || bail "Failed to copy OS binaries into directory: $extdir/$pkgname/$osname"
    separator
fi

# move libraries into os path
if [ -d "$prefix/lib" ]
then
	report "Copying OS dependent libraries for '$pkgname'"
	separator
    mkdir -p "$extdir/$pkgname/lib/$osname"
	cp -TRv  "$prefix/lib" "$extdir/$pkgname/lib/$osname" || bail "Failed to copy OS binaries into directory: $extdir/$pkgname/$osname"
	separator
fi

# move binaries into os path
if [ -d "$prefix/bin" ]
then
	report "Copying OS dependent binaries for '$pkgname'"
	separator
    mkdir -p "$extdir/$pkgname/bin/$osname"
    cp -TRv  "$prefix/bin" "$extdir/$pkgname/bin/$osname" || bail "Failed to copy OS binaries into directory: $extdir/$pkgname/$osname"
    separator
fi

if [ "$pkgkeep" -eq 1 ]
then
    report "Keeping package build directory for '$pkgbase'"
else
    report "Removing package build directory for '$pkgbase'"
    rm -r "$prefix"
    separator
fi

report "DONE building '$pkgname' from '$pkgfile'! --"
separator
