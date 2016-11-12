/**
  File    		: lab1.c
  Author  		: Sahil Khan
  Description	: This is lab1 in the series of lab for Module Programming, this basically introduces
                  the concept of loading and unloading of the module, as well as shows two ipmortant 
                  funtions that are called when module is loaded and unloaded.The logs can be seen at 
                  /var/log/kern.log or do a dmesg on the console.
*/


#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Khan");

static int __init hello_init(void)
{	
	printk(KERN_INFO"I am in INIT module\n");
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_INFO"I am in EXIT module\n");
}

module_init(hello_init);
module_exit(hello_exit);
