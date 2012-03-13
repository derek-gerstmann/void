#!/bin/bash

####################################################################################################

function build_ext()
{
    local dir="./ext"
    if [[ -d $dir ]]
    then
        local builder
        for builder in "$dir"/*
        do
            if [[ -f $builder && $(basename $builder) != 'common' ]]
            then
                eval $builder
            fi
        done
    fi
}

####################################################################################################

build_ext