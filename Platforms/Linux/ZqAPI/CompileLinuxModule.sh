#!/usr/bin/sh

# This script should run on the source directory.

# The module sources, relative to this directory/SOMETHING
export ZQ_MODULE_SOURCES=$1
# The module's objects, the same as the previous.
export ZQ_OBJECTS=$2
#
export ZQ_LINUX_VERSION=$4

# Output directory
export ZQ_OUT_DIR=$5

# The module's name
export name=$6
makefile=$3

export ZQ_COMPILER_ARGUMENTS=$7

#echo $ZQ_OUT_DIR
#echo $makefile
#echo $ZQ_MODULE_SOURCES
#echo $ZQ_OBJECTS

#REAL_OUTPUT_DIR="$ZQ_OUT_DIR/$name""Output"

#rm -rf "$REAL_OUTPUT_DIR"
# Create the output directory if it isn't exist.
#mkdir "$REAL_OUTPUT_DIR"

# Copy the module source files to the output directory.
#cp $ZQ_MODULE_SOURCES "$REAL_OUTPUT_DIR"

# For some reason, Linux build system needs the makefile to be in the output directory.
# Copy it everytime.
cp "$makefile" "$ZQ_OUT_DIR"

# Enter the directory.
#cd "$REAL_OUTPUT_DIR"

#ls $ZQ_MODULE_SOURCES

make -f "$makefile" && cp "$name.ko" ../
# && cp "$REAL_OUTPUT_DIR/$name.ko" $ZQ_OUT_DIR
