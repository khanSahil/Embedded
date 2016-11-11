#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

int sum(int a,int b);
EXPORT_SYMBOL(sum);


static int __init add_init(void)
{	
	printk(KERN_INFO"INIT_MODULE ADD: Inside the Add Init Module\n");
	return 0;
}
static void __exit add_exit(void)
{
	printk(KERN_INFO"EXIT_MODULE ADD: Inside the Add Exit Module\n");
}

int sum(int a, int b)
{
	return a+b;	
}
module_init(add_init);
module_exit(add_exit);

