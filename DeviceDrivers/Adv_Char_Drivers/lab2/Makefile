obj-m := lab2.o

KERN_SRC :=/lib/modules/$(shell uname -r)/build
PWD := /home/sahil/DeviceDrivers/myprog/Adv_Char_Drivers/lab2

modules:
		make -C $(KERN_SRC) M=$(PWD) modules
modules_install:
		make -C $(KERN_SRC) M=$(PWD) modules_install
		depmod -a

clean:
		make -C $(KERN_SRC) M=$(PWD) clean


