obj-m += pf.o
KERNEL_DIR := /lib/modules/`uname -r`/build
MODULEDIR := $(shell pwd)

modules:
	make -C $(KERNEL_DIR) M=$(MODULEDIR) modules