#!/usr/bin/sh

export ZQ_MODULE_SOURCES=$1
export ZQ_OBJECTS=$2
export ZQ_LINUX_VERSION=$4

# Output directory
export ZQ_OUT_DIR=$5

# The module's name
export name=$6
makefile=$3

export ZQ_SRC_DIR=$7

#echo $ZQ_OUT_DIR
#echo $makefile
#echo $ZQ_MODULE_SOURCES
#echo $ZQ_OBJECTS

REAL_OUTPUT_DIR="$ZQ_OUT_DIR/$name""Output"

rm -rf "$REAL_OUTPUT_DIR"
mkdir "$REAL_OUTPUT_DIR"

cp -r $PWD/* $REAL_OUTPUT_DIR
cd $REAL_OUTPUT_DIR

#ls $ZQ_MODULE_SOURCES

make -f "$makefile" && cp "$REAL_OUTPUT_DIR/$name.ko" $ZQ_OUT_DIR
