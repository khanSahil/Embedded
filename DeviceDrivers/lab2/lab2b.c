#include<linux/module.h>
#include<linux/moduleparam.h>
MODULE_LICENSE("GPL");

static struct myStruct
{
	int a;
	int b;
}s1 = {1,2};

//module_param(s1.a,int,S_IRUGO);
//module_param(s1.b,int,S_IRUGO);

static int __init hello_init(void)
{	
	printk(KERN_INFO"INIT_MODULE: The sum of the two numbers is %d\n",s1.a+s1.b);
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_INFO"EXIT_MODULE: The difference of the two numbers is %d\n",s1.a-s1.b);
}
module_init(hello_init);
module_exit(hello_exit);
