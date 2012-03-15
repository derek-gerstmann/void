#!/bin/bash

####################################################################################################

export ABSPATH=$(cd ${0%/*} && echo $PWD)
export ID=$( basename "$ABSPATH" )

####################################################################################################

# setup project paths
abs_cwd="$( cd "$( dirname "$0" )/.." && pwd )"
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
    echo "Please execute package build script from within the void/scripts subfolder: '$abs_cwd'!"
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

function checkout()
{
    local url=$1
    local dir=$2

    url="${url/svn/http}"
    local cmd="$(which svn)"
    if [ -e $cmd ];
    then
        cmd="svn checkout"
        $cmd $url $dir || bail "Failed to checkout '$url' into '$dir'"
    else
        bail "Failed to locate 'svn' command!  Please install this command line utility!"
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

function cp_dir()
{
    local src=$1
    local dst=$2

    if [ "$is_lnx" -eq 1 ]
    then
        # echo cp -TRv $src $dst
        cp -TRv $src $dst || bail "Failed to copy from '$src' to '$dst'"
    elif [ "$is_osx" -eq 1 ]
    then
        # echo ditto -v $src $dst
        ditto -v $src $dst || bail "Failed to copy from '$src' to '$dst'"
    fi
}

####################################################################################################

function run_all()
{
    local dir="$1"
    if [[ -d $dir ]]
    then
        local builder
        for builder in "$dir"/*
        do
            if [[ -f $builder ]]
            then
                eval $builder
            fi
        done
    fi
}

function setup_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    separator
    report "Setting up external package '$pkg_base' for '$os_name' ... "
    mkdir -p "$pkg_dir"
    cd "$pkg_dir"
    separator

    # remove any existing extracted pkg folder
    if [ -d "$pkg_base" ]
    then
        report "Removing old package '$pkg_base'"
        chmod -R +rw "$pkg_base"
        rm -r "$pkg_base"
        separator
    fi
}

function fetch_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    # if a local copy doesn't exist, grab the pkg from the url
    if [ ! -f "$pkg_file" ]
    then
        report "Retrieving package '$pkg_name' from '$pkg_url'"
        if [[ $(echo "$pkg_url" | grep -c 'http://') == 1 ]]
        then
            fetch $pkg_url $pkg_file 
        fi
        if [[ $(echo "$pkg_url" | grep -c 'git://') == 1 ]]
        then
            clone $pkg_url $pkg_base 
            make_archive $pkg_file $pkg_base
        fi
        if [[ $(echo "$pkg_url" | grep -c 'svn://') == 1 ]]
        then
            checkout $pkg_url $pkg_base 
            make_archive $pkg_file $pkg_base
        fi
        separator
    fi

    # extract any pkg archives
    if [[ $(is_archive "$pkg_file") == 1 ]]
    then
        report "Extracting package '$pkg_file'"
        extract_archive $pkg_file
    fi
}

function boot_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    cd "$pkg_base"
    separator

    # bootstrap package
    if [ ! -f "./configure" ]
    then
        if [ -f "./bootstrap" ]
        then
            report "Bootstraping package '$pkg_name'"
            separator
            ./bootstrap
            separator
        fi
        if [ -f "./bootstrap.sh" ]
        then 
            report "Bootstraping package '$pkg_name'"
            separator
            ./bootstrap.sh
            separator
        fi
        if [ -f "./autogen.sh" ]
        then 
            report "Bootstraping package '$pkg_name'"
            separator
            ./autogen.sh
            separator
        fi
    fi

}

function cmake_pkg()
{
    # configure package
    local m=7
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_mpath=$5
    local pkg_env=$6
    local pkg_cfg="${@:$m}"

    cd "$pkg_base"
    mkdir -p build
    cd build

    separator
  
    local prefix="$ext_dir/build/$pkg_name/$os_name"

    local env_flags=" "
    if [ -n $pkg_mpath ] && [ $pkg_mpath != 0 ]
    then
        pkg_mpath=$(echo $pkg_mpath | split_str ":" | join_str " ")
        env_flags='-DCMAKE_MODULE_PATH="'$pkg_mpath'"'
    fi
        
    if [ -n $pkg_env ] && [ $pkg_env != 0 ]
    then
        pkg_env=$(echo $pkg_env | split_str ":" | join_str " ")
        env_flags=$env_flags' '$pkg_env
    fi

    local cmake_src_path=".."
    local src_paths=".. ../src ../source"
    for path in ${src_paths}
    do
        if [ -f "$path/CMakeLists.txt" ] 
        then
            cmake_src_path="$path"
        fi
    done

    report "Configuring package '$pkg_name' from source folder '$cmake_src_path' ..."
    separator

    local cmake_pre="-DCMAKE_INSTALL_PREFIX=$prefix"
    if [ "$is_osx" -eq 1 ]
    then
        cmake_pre="$cmake_pre -DCMAKE_OSX_ARCHITECTURES=x86_64"
    fi
    echo cmake $cmake_pre $env_flags $cmake_src_path
    separator

    eval cmake $cmake_pre $env_flags $cmake_src_path || bail "Failed to configure: '$prefix'"
    separator

    report "Done configuring package '$pkg_name'"
}

function cfg_pkg()
{
    # configure package
    local m=7
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_cflags=$5
    local pkg_ldflags=$6
    local pkg_cfg="${@:$m}"

    local prefix="$ext_dir/build/$pkg_name/$os_name"

    if [ -f "./configure" ]
    then

        local env_flags=" "

        if [ -n $pkg_cflags ] && [ $pkg_cflags != 0 ]
        then
            pkg_cflags=$(echo $pkg_cflags | split_str ":" | join_str " ")
            env_flags='CXXFLAGS="'$pkg_cflags'" CPPFLAGS="'$pkg_cflags'" CFLAGS="'$pkg_cflags'"'
        fi

        if [ -n $pkg_ldflags ] && [ $pkg_ldflags != 0 ]
        then
            pkg_ldflags=$(echo $pkg_ldflags | split_str ":" | join_str " ")
            env_flags=$env_flags' LDFLAGS="'$pkg_ldflags'"'
        fi
        
        report "Configuring package '$pkg_name' ..."
        separator

        echo ./configure --prefix="$prefix" $pkg_cfg $env_flags 
        separator

        eval ./configure --prefix="$prefix" $pkg_cfg $env_flags || bail "Failed to configure: '$prefix'"
        separator

        report "Done configuring package '$pkg_name'"

    else
        if [ -f "./CMakeLists.txt" ] || [ -f "./src/CMakeLists.txt" ] || [ -f "./source/CMakeLists.txt" ]
        then
            cmake_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cflags $pkg_ldflags $pkg_cfg
        fi
    fi
}

function make_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    local prefix="$ext_dir/build/$pkg_name/$os_name"

    # build using make if a makefile exists
    if [ -f "./Makefile" ] || [ -f "./makefile" ]
    then
        report "Building package '$pkg_name'"
        separator
        make  || bail "Failed to build package: '$prefix'"
        separator
    fi
}

function install_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    local prefix="$ext_dir/build/$pkg_name/$os_name"

    # extract the makefile rule names and filter out empty lines and comments
    local rules=$(make -pn | grep -v ^$ | grep -v ^# | grep -c '^install:')

    # install using make if an 'install' rule exists
    if [[ $(echo "$rules") > 0 ]]
    then
        report "Installing package '$pkg_name'"
        separator
        make install || bail "Failed to install package: '$prefix'"
        separator
    fi

}

function migrate_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5
    local prefix="$ext_dir/build/$pkg_name/$os_name"

    report "Migrating package '$pkg_name'"
    separator

    local src_paths="include inc man share"
    for path in ${src_paths}
    do
        # move product into external path
        if [ -d "$prefix/$path" ]
        then
            report "Copying build products from '$prefix/$path' for '$pkg_name'"
            separator
            mkdir -p "$ext_dir/$pkg_name/$path"
            cp_dir "$prefix/$path" "$ext_dir/$pkg_name/$path" || bail "Failed to copy shared files into directory: $ext_dir/$pkg_name/$os_name"
            separator
        fi
    done

    local bin_paths="lib bin lib32 lib64"
    for path in ${bin_paths}
    do
        # move product into external os specific path
        if [ -d "$prefix/$path" ]
        then
            report "Copying build products from '$prefix/$path' for '$pkg_name'"
            separator
            mkdir -p "$ext_dir/$pkg_name/$path/$os_name"
            cp_dir "$prefix/$path" "$ext_dir/$pkg_name/$path/$os_name" || bail "Failed to copy OS binaries into directory: $ext_dir/$pkg_name/$os_name"
            separator
        fi
    done

    if [ "$pkg_keep" -eq 1 ]
    then
        report "Keeping package build directory for '$pkg_base'"
    else
        report "Removing package build directory for '$pkg_base'"
        chmod -R +rw "$prefix"
        rm -r "$prefix"
        separator
    fi
}

####################################################################################################

function build_pkg()
{
    local m=8
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5
    local pkg_cflags=$6
    local pkg_ldflags=$7
    local pkg_cfg="${@:$m}"

#    echo "PkgName: '$pkg_name' PkgBase: '$pkg_base' PkgFile: '$pkg_file' PkgUrl: '$pkg_url' PkgCfg: '$pkg_cfg' PkgKeep: '$pkg_keep'"

    setup_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    fetch_pkg   $pkg_name $pkg_base $pkg_file $pkg_url
    boot_pkg    $pkg_name $pkg_base $pkg_file $pkg_url
    cfg_pkg     $pkg_name $pkg_base $pkg_file $pkg_url $pkg_cflags $pkg_ldflags $pkg_cfg
    make_pkg    $pkg_name $pkg_base $pkg_file $pkg_url
    install_pkg $pkg_name $pkg_base $pkg_file $pkg_url
    migrate_pkg $pkg_name $pkg_base $pkg_file $pkg_url $pkg_keep

    report "DONE building '$pkg_name' from '$pkg_file'! --"
    separator
}

####################################################################################################
