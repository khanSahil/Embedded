#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/semaphore.h>
#include<asm/atomic.h>
#include<asm/current.h>
#include<asm/string.h>
#include<linux/sched.h>
#include<linux/interrupt.h>
#define SUCCESS 0

MODULE_LICENSE("GPL");

int	dev_open(struct inode *, struct file *);
int dev_release(struct inode *, struct file *);
ssize_t dev_read(struct file *, char __user *, size_t,loff_t *);

struct file_operations fops={
.owner=THIS_MODULE,
.open=dev_open,
.read=dev_read,
.release=dev_release,
};

dev_t deviceno;
struct cdev *charDrive;
struct tasklet_struct task;

void myFunction(unsigned long data)
{
	printk("Hello World : %d\n",data);
}


static int __init chardrive_init(void)
{
	printk("chardriver init functions\n");

	char deviceName[] = "CharacterDrive";
	int result,majorNum,minorNum = 0,count = 1;

	result=alloc_chrdev_region(&deviceno,minorNum,count,deviceName);
	
	if(result<0)
		printk(KERN_ERR"registration of deviceno failed\n");

	majorNum = MAJOR(deviceno); 
	minorNum = MINOR(deviceno);
	printk("The major number is %d\nthe minor number is %d\n",majorNum,minorNum);
	
	charDrive=cdev_alloc();
	charDrive->ops=&fops;								
	result=cdev_add(charDrive,deviceno,count);
						
	if(result<0)
		printk(KERN_ERR"Addition of Device failed\n");
	return 0;
}


static void __exit chardrive_exit(void)
{
	printk("chardriver exit function\n");
	unregister_chrdev_region(deviceno,1);
	tasklet_disable(&task);
	tasklet_kill(&task);
	cdev_del(charDrive);
}
module_init(chardrive_init);
module_exit(chardrive_exit);


int dev_open(struct inode *inod,struct file *filp)
{
	printk("chardriver open function\n");
	return 0;
}

int dev_release(struct inode *inod,struct file *filp)
{
	printk("chardriver close function\n");
	return 0;
}

ssize_t dev_read(struct file *fp,char *uBuff,size_t count,loff_t *ofs)
{
	ssize_t retval;
	unsigned long data = 1;
	if(strcmp(current->comm,"read1")==0){
		tasklet_init(&task,myFunction,data);
		tasklet_enable(&task);
		tasklet_schedule(&task);
		return SUCCESS;
	}	
}
