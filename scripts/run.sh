#!/bin/bash

set -euo pipefail

base=$(basename $(pwd))

print_style () {
    if [ "$2" == "info" ] ; then
        COLOR="96m";
    elif [ "$2" == "success" ] ; then
        COLOR="92m";
    elif [ "$2" == "warning" ] ; then
        COLOR="93m";
    elif [ "$2" == "error" ] ; then
        COLOR="91m";
    else #default color
        COLOR="0m";
    fi

    STARTCOLOR="\e[$COLOR";
    ENDCOLOR="\e[0m";

    printf "$STARTCOLOR%b$ENDCOLOR" "$1\n";
}

if [[ $base == "scripts" ]];
then
  print_style "You must run this script from the base dir as scripts/run.sh" "error";
  exit 1;
fi

cmake --build $(pwd)/build --target voxel && /$(pwd)/build/voxel
