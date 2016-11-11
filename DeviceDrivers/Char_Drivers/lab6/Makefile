obj-m := lab6.o

KERN_SRC :=/lib/modules/$(shell uname -r)/build
PWD := /home/sahil/DeviceDrivers/myprog/Char_Drivers/lab6

modules:
		make -C $(KERN_SRC) M=$(PWD) modules
modules_install:
		make -C $(KERN_SRC) M=$(PWD) modules_install
		depmod -a

clean:
		make -C $(KERN_SRC) M=$(PWD) clean


