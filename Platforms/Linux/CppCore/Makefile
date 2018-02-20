ZQ_LINUX_MODULE_OBJECT := $(addsuffix .o,$(basename $(ZQ_MODULE_SOURCES)))
OTHER_MODULE_OBJECTS=$(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT))

name := $(name)
obj-m := $(name).o
ko-m := $(name).ko
#Linux-objs := $(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT)) DummyCPPForwarder.o
#ZQ_OBJECTS := $(addprefix ${sandbox_pwd}/, $(ZQ_OBJECTS))
$(name)-objs := $(OTHER_MODULE_OBJECTS) $(ZQ_OBJECTS)

ccflags-y := $(addprefix -I${sandbox_pwd}/, $(ZQ_INCLUDE_PATHS))

#Linux-objs := ../../../../Build/Base/libBase.a
#EXTRA_CFLAGS+=-DLINUX_DRIVER -mhard-float
#EXTRA_CFLAGS := "-I/usr/include/"

#obj=$(ZQ_OUT_DIR)

#CURDIR=/lib/modules/${ZQ_LINUX_VERSION}/build
#$(info $(M))
#override M := $(PWD)/$(ZQ_MAKEFILE_PATH)
#$(info $(M))
#make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

zq_info : ;    make -s -C "${ZQ_LINUX_HEADERS_PATH}" M="${sandbox_pwd}/${ZQ_MAKEFILE_PATH}" -k --no-print-directory --eval='$$(info $$(KBUILD_CFLAGS))'
zq_modules : ; make -C "${sandbox_pwd}/${ZQ_LINUX_HEADERS_PATH}" M="${sandbox_pwd}/${ZQ_MAKEFILE_PATH}" modules

#ziqe_modules: modules ; env
#ld -r --build-id -o $(ko-m).new Linux.mod.o $(OTHER_OBJECTS) $(ZQ_OBJECTS)

#clean: ; make -B -C /lib/modules/$(ZQ_LINUX_VERSION)/build M=$(ZQ_OUT_DIR) clean
