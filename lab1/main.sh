# !/bin/bash

if [ $# -eq 1 ]
then
    if [ -d $1 ]
    then
        if [ ${1: -1} == "/" ]
        then
            path="$1*"
        else
            path="$1/*"
        fi

        for file in $path
        do
            if [ -d $file ]
            then
                printf 'Directory - %s\n' "$(basename $file)"
            fi
        done

        printf '\n'

        for file in $path
        do
            if [ -f $file ]
            then
                printf 'File - %s\n' "$(basename $file)"
                printf '%s\n\n' "$(stat --printf="Size - %s bytes\nTime - %y\nLinks - %h" $file)"
            fi
        done
    else
        printf '%s cannot be opened.' "$1"
    fi
else
    printf 'Wrong number of arguments (expected 1, given %d).' "$#"
fi