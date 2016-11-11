#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
MODULE_LICENSE("GPL");

#define SUCCESS 0

dev_t deviceNum;

static int __init hello_init(void)
{	
	printk("I am in INIT module\n");
	char myDevice[] = "chardevice";
	int result, majorNum, minorNum = 0, noOfDevices= 1;
	result = alloc_chrdev_region(&deviceNum,minorNum,noOfDevices,myDevice);	

	if(result < 0)
	{
		printk(KERN_ERR"ERROR : Device Registration Fails\n");
		return result;
	}
	else
	{
		printk("Device Registration is Successfull\n");
		majorNum = MAJOR(deviceNum);
		minorNum = MINOR(deviceNum);
		printk("Device Major Number: %d, Minor Number : %d\n",majorNum,minorNum);
		return SUCCESS;
	}	
}
static void __exit hello_exit(void)
{
	printk("I am in cleanup module\n");
	unregister_chrdev_region(deviceNum,1);
	printk("Device Unregistration is Done\n");
}

module_init(hello_init);
module_exit(hello_exit);
