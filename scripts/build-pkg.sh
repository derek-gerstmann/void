#!/bin/bash

####################################################################################################

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

    prefix="$ext_dir/build/$pkg_name/$os_name"
    if [ -f "./configure" ]
    then
        env_flags=" "
        if [ -n $pkg_cflags ] && [ $pkg_cflags != 0 ]
        then
            pkg_cflags=$(echo $pkg_cflags | split_str ":" | join_str " ")
            env_flags='CXXFLAGS="'$pkg_cflags'" CFLAGS="'$pkg_cflags'"'
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
    fi
}

function make_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    # build and install into local path
    prefix="$ext_dir/build/$pkg_name/$os_name"
    report "Building package '$pkg_name'"
    separator
    make  || bail "Failed to build package: '$prefix'"
    separator

}

function install_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4

    prefix="$ext_dir/build/$pkg_name/$os_name"
    report "Installing package '$pkg_name'"
    separator
    make install || bail "Failed to install package: '$prefix'"
    separator
}

function migrate_pkg()
{
    local pkg_name=$1
    local pkg_base=$2
    local pkg_file=$3
    local pkg_url=$4
    local pkg_keep=$5

    prefix="$ext_dir/build/$pkg_name/$os_name"
    report "Migrating package '$pkg_name'"
    separator

    # move header into external path
    if [ -d "$prefix/include" ]
    then
        report "Copying headers for '$pkg_name'"
        separator
        mkdir -p "$ext_dir/$pkg_name/include"
        cp -TRv  "$prefix/include" "$ext_dir/$pkg_name/include" || bail "Failed to copy OS binaries into directory: $ext_dir/$pkg_name/$os_name"
        separator
    fi

    # move libraries into os path
    if [ -d "$prefix/lib" ]
    then
        report "Copying OS dependent libraries for '$pkg_name'"
        separator
        mkdir -p "$ext_dir/$pkg_name/lib/$os_name"
        cp -TRv  "$prefix/lib" "$ext_dir/$pkg_name/lib/$os_name" || bail "Failed to copy OS binaries into directory: $ext_dir/$pkg_name/$os_name"
        separator
    fi

    # move binaries into os path
    if [ -d "$prefix/bin" ]
    then
        report "Copying OS dependent binaries for '$pkg_name'"
        separator
        mkdir -p "$ext_dir/$pkg_name/bin/$os_name"
        cp -TRv  "$prefix/bin" "$ext_dir/$pkg_name/bin/$os_name" || bail "Failed to copy OS binaries into directory: $ext_dir/$pkg_name/$os_name"
        separator
    fi

    if [ "$pkg_keep" -eq 1 ]
    then
        report "Keeping package build directory for '$pkg_base'"
    else
        report "Removing package build directory for '$pkg_base'"
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
