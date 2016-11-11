#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
MODULE_LICENSE("GPL");

static int a,b;
module_param(a,int,S_IRUGO);
module_param(b,int,S_IRUGO);


static int __init hello_init(void)
{	
	printk(KERN_INFO"INIT_MODULE: The sum of the two numbers is %d\n",a+b);
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_INFO"EXIT_MODULE: The difference of the two numbers is %d\n",a-b);
}
module_init(hello_init);
module_exit(hello_exit);
