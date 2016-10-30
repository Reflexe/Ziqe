#!/usr/bin/sh

export ZQ_MODULE_SOURCES=$1
export ZQ_OBJECTS=$2
export ZQ_LINUX_VERSION=$4

# Output directory
#export O=$5

# The module's name
export name=$6
makefile=$3


#echo $O
#echo $makefile
#echo $ZQ_MODULE_SOURCES
#echo $ZQ_OBJECTS

make -f "$makefile"
