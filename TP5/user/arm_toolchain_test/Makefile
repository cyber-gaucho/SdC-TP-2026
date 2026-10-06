CC=arm-linux-gnueabihf-gcc

# Flags for ARMv6 architecture, using the ARM instruction set, VFP floating-point unit, and hard float ABI
CFLAGS=-march=armv6 -marm -mfpu=vfp -mfloat-abi=hard

all:
	$(CC) $(CFLAGS) hello-world.c -o hello-world

clean:
	rm -f hello-world
