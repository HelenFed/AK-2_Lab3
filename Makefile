ifneq ($(KERNELRELEASE),)
	obj-m := hello.o
else
	KDIR ?= /lib/modules/`uname -r`/build
default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	$(MAKE) -C $(KDIR) N=$$PWD clean
endif
