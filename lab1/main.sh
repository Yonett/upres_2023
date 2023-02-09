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
                echo $file
            fi
        done

        for file in $path
        do
            if [ -f $file ]
            then
                echo $(stat --format="%n %s %y %h" $file)
            fi
        done
    else
        echo "$1 is not a directory"
    fi
else
    echo "Wrong number of arguments (expected 1, given $#)"
fi