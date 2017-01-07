ZQ_LINUX_MODULE_OBJECT := $(addsuffix .o,$(basename $(ZQ_MODULE_SOURCES)))
OTHER_MODULE_OBJECTS=$(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT))

name := $(name)
obj-m := $(name).o
ko-m := $(name).ko
#Linux-objs := $(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT)) DummyCPPForwarder.o
$(name)-objs := $(OTHER_MODULE_OBJECTS) $(ZQ_OBJECTS)
ccflags-y := -I$(ZQ_SRC_DIR) -I.

#Linux-objs := ../../../../Build/Base/libBase.a
#EXTRA_CFLAGS+=-DLINUX_DRIVER -mhard-float
#EXTRA_CFLAGS := "-I/usr/include/"

#obj=$(ZQ_OUT_DIR)

all: ;  touch "$(CURDIR)/$(name).c" ; make -B -C /lib/modules/$(ZQ_LINUX_VERSION)/build M=$(CURDIR) modules
#ld -r --build-id -o $(ko-m).new Linux.mod.o $(OTHER_OBJECTS) $(ZQ_OBJECTS)

clean: ; make -B -C /lib/modules/$(ZQ_LINUX_VERSION)/build M=$(CURDIR) clean
