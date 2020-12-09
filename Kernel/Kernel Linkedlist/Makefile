KERNELDIR=/lib/modules/`uname -r`/build

#MODULES = KernelModule.ko
obj-m += KernelModule.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
