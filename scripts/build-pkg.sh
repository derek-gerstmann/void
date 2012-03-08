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

function fetch() 
{
    local url=$1
    local tarball=$2
    local dir=$3

    local usepipe=0
    local cmd="$(which curl)"
    if [ -e $cmd ];
    then
    	cmd="curl -f -L -O "
    	usepipe=1
    else
    	cmd="$(which wget)"
	    if [ ! -e $cmd ];
	    then
	    	bail "Failed to locate either 'wget' or 'curl' for fetching packages!  Please install one of these utilties!"
	    else
	    	$cmd="wget --progress=dot "
	    fi
    fi

    if [ ! -e $tarball ]; then
        report "Fetching '$tarball'..."
        separator
        if [ "$usepipe" -eq 1 ]
        then
			$cmd $url > $tarball
		else
	        $cmd
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
    
    tar cjf "${tarball}" "${dir}" &>${LOG_FILE} || bail "Failed to create tarball!"
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

# move into external pkg folder for retrieving pkgs
separator
report "Setting up external package '$pkgbase' for '$osname' ... "
cd "$extdir"
mkdir -p pkgs
cd pkgs
rm -r "$extdir/$pkgname"
separator

# remove any existing extracted pkg folder
if [ -d "$pkgbase" ]
then
	report "Removing old package '$pkgbase'"
	rm -r "$pkgbase"
fi

# if a local copy doesn't exist, grab the pkg from the url
if [ ! -f "$pkgfile" ]
then
	report "Retrieving package '$pkgname'"
	fetch $pkgurl $pkgfile 
fi

# extract pkg file and move into extracted folder
report "Extracting package '$pkgfile'"
extract_tarball $pkgfile
cd "$pkgbase"
separator

# configure package
report "Configuring package '$pkgname'"
separator
echo ./configure --prefix="$extdir/$pkgname" $pkgcfg
separator
./configure --prefix="$extdir/$pkgname" $pkgcfg || bail "Failed to configure: $extdir/$pkgname"
separator
report "Done configuring package '$pkgname'"

# build and install into local path
report "Building package '$pkgname'"
make  || bail "Failed to build package: $extdir/$pkgname"
separator

report "Installing package '$pkgname'"
make install || bail "Failed to install package: $extdir/$pkgname"
separator

# move libraries into os path
if [ -d "$extdir/$pkgname/lib" ]
then
	report "Moving OS dependent libraries for '$pkgname'"
	separator
	cp -R  "$extdir/$pkgname/lib" "$extdir/$pkgname/$osname" || bail "Failed to copy OS binaries into directory: $extdir/$pkgname/$osname"
	rm -r  "$extdir/$pkgname/lib" 					 		 || bail "Failed to remove old directory: $extdir/$pkgname/lib"
	mkdir -p "$extdir/$pkgname/lib/$osname"  || bail "Failed to create OS binary directory: $extdir/$pkgname/lib"
	cp -Rv "$extdir/$pkgname/$osname" "$extdir/$pkgname/lib/$osname"  || bail "Failed to move OS binaries into directory: $extdir/$pkgname/lib"
	rm -r  "$extdir/$pkgname/$osname" 					 	 || bail "Failed to remove old directory: $extdir/$pkgname/$osname"
	separator
fi

# move binaries into os path
if [ -d "$extdir/$pkgname/bin" ]
then
	report "Moving OS dependent binaries for '$pkgname'"
	separator
	cp -R  "$extdir/$pkgname/bin" "$extdir/$pkgname/$osname" || bail "Failed to copy OS binaries into directory: $extdir/$pkgname/$osname"
	rm -r  "$extdir/$pkgname/bin" 					 		 || bail "Failed to remove old directory: $extdir/$pkgname/bin"
	mkdir -p "$extdir/$pkgname/bin/$osname"  || bail "Failed to create OS binary directory: $extdir/$pkgname/bin"
	cp -Rv "$extdir/$pkgname/$osname" "$extdir/$pkgname/bin/$osname"  || bail "Failed to move OS binaries into directory: $extdir/$pkgname/bin"
	rm -r  "$extdir/$pkgname/$osname" 					 	 || bail "Failed to remove old directory: $extdir/$pkgname/$osname"
	separator
fi

report "DONE building '$pkgname' from '$pkgfile'! --"
separator
