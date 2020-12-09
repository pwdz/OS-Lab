KERNELDIR=/lib/modules/$(shell uname -r)/build

#MODULES = Sample.ko
obj-m += Sample.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
