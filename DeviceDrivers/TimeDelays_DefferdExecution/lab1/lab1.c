#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include <linux/ioctl.h>
#include<asm/ioctl.h>
#include<linux/sched.h>
#include "app.h"
#define SUCCESS 0

MODULE_LICENSE("GPL");

int	dev_open(struct inode *, struct file *);
int dev_release(struct inode *, struct file *);
ssize_t dev_read(struct file *, char __user *, size_t,loff_t *);
ssize_t dev_write(struct file *, char __user *, size_t,loff_t *);
int dev_ioctl(struct file*, unsigned int ,unsigned long);

struct file_operations fops={
.owner=THIS_MODULE,
.open=dev_open,
.unlocked_ioctl= dev_ioctl,
.release=dev_release,
};

dev_t deviceno;
struct cdev *charDrive;
wait_queue_head_t wait;
unsigned long delay;

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
	init_waitqueue_head(&wait);
						
	if(result<0)
		printk(KERN_ERR"Addition of Device failed\n");
	return 0;
}


static void __exit chardrive_exit(void)
{
	printk("chardriver exit function\n");
	unregister_chrdev_region(deviceno,1);
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


int dev_ioctl(struct file *filp,unsigned int cmd,unsigned long int arg)
{
	int retValue;
	printk("Inside IOCTL Function\n");
	switch(cmd)
	{
		case SET_DELAY:
			printk("Setting the Delay : %d\n",*(int*)arg);
			retValue = get_user(delay,(int *)arg);
			delay = delay * HZ;
			if(retValue == SUCCESS){
				printk("Setting the Jiffies in Kernel Successfully %d\n",delay);
			}
			else{
				printk("ERROR: Jiffies not set in Kernel correctly\n");
			}
			wait_event_interruptible_timeout(wait,0,delay);
		break;
		default:
			printk("Invalid Command\n");
			return -1;
	}
	return SUCCESS;	
}
