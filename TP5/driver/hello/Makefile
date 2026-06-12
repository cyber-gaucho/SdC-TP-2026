ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-

KDIR := ../../toolchain/kernel-export-v2/linux-headers-6.12.75+rpt-rpi-v6

obj-m += hello.o

all:
	make -C $(KDIR) \
		M=$(PWD) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		modules

clean:
	make -C $(KDIR) \
		M=$(PWD) \
		ARCH=$(ARCH) \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		clean