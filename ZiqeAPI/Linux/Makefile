ZQ_LINUX_MODULE_OBJECT := $(addsuffix .o,$(basename $(ZQ_MODULE_SOURCES)))
OTHER_MODULE_OBJECTS=$(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT))

name := Linux
obj-m := Linux.o
ko-m := Linux.ko
#Linux-objs := $(filter-out Linux.o,$(ZQ_LINUX_MODULE_OBJECT)) DummyCPPForwarder.o
Linux-objs := $(OTHER_MODULE_OBJECTS) $(ZQ_OBJECTS)
#Linux-objs := ../../../../Build/Base/libBase.a
#EXTRA_CFLAGS+=-DLINUX_DRIVER -mhard-float
#EXTRA_CFLAGS := "-I/usr/include/"

all: ; \
make -B -C /lib/modules/$(ZQ_LINUX_VERSION)/build M=$(CURDIR) modules |& grep "WARNING" >&2 && exit 1
#ld -r --build-id -o $(ko-m).new Linux.mod.o $(OTHER_OBJECTS) $(ZQ_OBJECTS)

clean: ; make -B -C /lib/modules/$(ZQ_LINUX_VERSION)/build M=$(CURDIR) clean
