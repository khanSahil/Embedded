#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
MODULE_LICENSE("GPL");

#define SUCCESS 0

dev_t deviceNum;

static int __init hello_init(void)
{	
	printk("I am in INIT module\n");
	char myDevice[] = "chardevice";
	
	deviceNum = MKDEV(255,1);
	
	if(register_chrdev_region(deviceNum,1,myDevice) == SUCCESS)
	{
		printk("Device Registration is Successfull\n");
	}
	else
	{
		printk(KERN_ERR"ERROR : Device Registration Fails\n");
	}	
	return 0;
}
static void __exit hello_exit(void)
{
	printk("I am in cleanup module\n");
	unregister_chrdev_region(deviceNum,1);
	printk("Device Unregistration is Done\n");
}

module_init(hello_init);
module_exit(hello_exit);
