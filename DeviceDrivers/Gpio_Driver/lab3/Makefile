obj-m := lab3.o

KERN_SRC :=/lib/modules/$(shell uname -r)/build
PWD := /home/debian/My_LDD_Programs/Gpio_Driver/lab3

modules:
		make -C $(KERN_SRC) M=$(PWD) modules
modules_install:
		make -C $(KERN_SRC) M=$(PWD) modules_install
		depmod -a

clean:
		make -C $(KERN_SRC) M=$(PWD) clean


