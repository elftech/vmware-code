#if KERNELRELEASE is defined, we've been invoked from th
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
	obj-m := registerDevice.o
# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD  := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	echo $(MAKE)
endif
.PHONY:clean
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko
