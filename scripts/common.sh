#!/bin/bash

####################################################################################################

export ABSPATH=$(cd ${0%/*} && echo $PWD)
export ID=$( basename "$ABSPATH" )

####################################################################################################

# setup project paths
abs_cwd="$( cd "$( dirname "$0" )" && pwd )"
base_dir="$( basename "$abs_cwd" )"
root_dir="$( dirname "$abs_cwd" )"
ext_dir="$root_dir/external"
pkg_dir="$root_dir/external/pkgs"

os_name="unknown"
is_lnx=$( uname -s | grep -c Linux )
if [ "$is_lnx" -eq 1 ]
then
    os_name="lnx"
fi

is_osx=$( uname -s | grep -c Darwin )
if [ "$is_osx" -eq 1 ]
then
    os_name="osx"
fi

if [ "$os_name" == "unknown" ]
then
    echo "Operating system is unknown and not detected properly.  Please update detection routine!"
    exit 0
fi 

# ensure we are run inside of the scripts folder
if [ "$base_dir" != "scripts" ]
then
    echo "Please execute package build script from within the void/scripts subfolder!"
    exit 0
fi 

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

function quote_str
{
    local filename=$1

    sed $filename \
        -e 's#\\#\\\\#' \
        -e 's#/#\\/#' \
        -e 's#\.#\\.#' \
        -e 's#\*#\\*#' \
        -e 's#\[#\\[#'

    return $?
}

function split_str
{
    local DEFAULT_DELIMITER=" "

    local a_delimiter="${1:-$DEFAULT_DELIMITER}"
    local a_inputfile="$2"

    awk -F "$a_delimiter" \
    '{  for(i = 1; i <= NF; i++) {
            print $i
        }
    }' $a_inputfile

    return $?
}

function join_str
{
    local DEFAULT_DELIMITER=" "

    local a_delimiter="${1:-$DEFAULT_DELIMITER}"
    local a_inputfile="$2"

    awk -v usersep="$a_delimiter" '
    BEGIN{
        sep=""; # Start with no separator (before the first item)
    }
    {
        printf("%s%s", sep, $0);
        (NR == 1) && sep = usersep; # Separator is set after the first item.
    }
    END{
        print "" # Print a new line at the end.
    }' $a_inputfile
    
    return $?
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
    local archive=$2
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

    if [ ! -e $archive ]; then
        report "Fetching '$archive'..."
        separator
        if [ "$usepipe" -eq 1 ]
        then
            echo "$cmd $url > $archive"
            $cmd $url > $archive
        else
            echo "$cmd $archive $url"
            $cmd $archive $url
        fi
        separator
        report "Downloaded '$url' to '$archive'..."
    fi

    if [ ! -e $archive ]; then
        bail "Failed to fetch '$archive'..."
    fi
}

function is_archive()
{
    local is=0
    local archive=$1
    if test -f ${archive%.*}*.bz2 ; then
        is=1
    elif  test -f ${archive%.*}*.gz ; then
        is=1
    elif  test -f ${archive%.*}*.tgz ; then
        is=1
    elif  test -f ${archive%.*}*.zip ; then
        is=1
    fi  
    echo "$is"
}

function extract_archive()
{
    local archive=$1
    if test -f ${archive%.*}*.bz2 ; then
        tar jxf ${archive} || bail "Failed to extract archive '${archive}'"
    elif  test -f ${archive%.*}*.gz ; then
        tar zxf ${archive} || bail "Failed to extract archive '${archive}'"
    elif  test -f ${archive%.*}*.tgz ; then
        tar zxf ${archive} || bail "Failed to extract archive '${archive}'"
    elif  test -f ${archive%.*}*.zip ; then
        unzip -uo ${archive} || bail "Failed to extract archive '${archive}'"
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

function make_archive()
{
    local archive=$1
    local dir=$2
    
    tar cjf "${archive}" "${dir}" || bail "Failed to create archive!"
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
