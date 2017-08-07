#!/bin/env sh

# build the linux targets with {linux_headers_path}/build;
# compile it to the our Makefile's path and read
# our Makefile from stdin to make its path simpler.

# 'info' or 'compile'
mode=$1
makefile_path=$2
output_dirname=$3
output_filename="$name.ko"

make_mode=""

export sandbox_pwd=$PWD

if [[ "$mode" == "info" ]]; then
    make_mode="zq_info"
elif [[ "$mode" == "compile" ]]; then
    make_mode="zq_modules"
else
    echo "Invalid mode: $mode" 1>&2
    exit 1
fi

make_output=`make -s -f "-" "$make_mode" < "$makefile_path"`

if test $? -eq 1; then
    echo "$make_output"
    exit 1
fi

warnings=`echo "$make_output" | grep 'WARNING:'`
if test $? -eq 0; then
    echo "Error: Found a warning: " 1>&2
    echo "$warnings" 1>&2
    exit 1
fi

if [[ "$mode" == "info" ]]; then
    echo "$make_output"
else
    # Then, copy the built target (X.ko) to the output directory.
    cp -r "$(dirname "$makefile_path")/$output_filename" "$output_dirname"
fi
