obj-m += Linux.o
Linux-y += SystemCalls.o
Linux-objs := ../../../../Build/Base/libBase.a
ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
#EXTRA_CFLAGS+=-DLINUX_DRIVER -mhard-float

all: ;  make -C /lib/modules/$(shell uname -r)/build M=$(ROOT_DIR) modules

clean: ; make -C /lib/modules/$(shell uname -r)/build M=$(ROOT_DIR) clean
