#include<linux/init.h>
#include<linux/module.h>

MODULE_LICENSE("GPL");

int sum(int,int);

static int __init avg_init(void)
{	
	int s  = sum(6,4);
	int avg = s/2;
	printk(KERN_INFO"INIT_MODULE AVERAGE: The sum and avg of the two numbers is %d %d\n",s,avg);
	return 0;
}
static void __exit avg_exit(void)
{
	printk(KERN_INFO"EXIT_MODULE AVERAGE: in the EXIT module\n");
}

module_init(avg_init);
module_exit(avg_exit);
