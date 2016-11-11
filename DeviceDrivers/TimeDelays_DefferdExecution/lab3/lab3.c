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
#include<linux/timer.h>
#include<linux/jiffies.h>
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
struct timer_list tList;

void myFunction(unsigned long data)
{
	printk("Hello World : %d\n",data);
	init_timer(&tList);
	tList.expires = jiffies + tList.data*HZ;
	tList.function = myFunction;	
	add_timer(&tList);
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
	if(strcmp(current->comm,"read1")==0){
		init_timer(&tList);
		tList.data = 1;
		tList.expires = jiffies + tList.data*HZ;
		tList.function = myFunction;
		printk("Initialized timer structure:\n data : %d \n expire : %d \n",tList.data,tList.expires);
		add_timer(&tList);
		return SUCCESS;
	}
	if(strcmp(current->comm,"read2")==0){
		tList.data = 2;
		printk("Initialized timer structure:\n data : %d \n expire : %d \n",tList.data,tList.expires);
		return SUCCESS;
	}
	if(strcmp(current->comm,"read3")==0){
		printk("Deleting the Timer\n");
		del_timer(&tList);
		return SUCCESS;
	}
	
	
/*	if(strcmp(current->comm,"read2")==0){
		result=copy_from_user((char*)&kBuff,(char*)uBuff,sizeof(uBuff));
		if(result==0){	
			printk("Data successfully written to kernel by write1 Process %d\n",kBuff);
			return SUCCESS;
		}
	}
	
	if(strcmp(current->comm,"read3")==0){
		result=copy_from_user((char*)&kBuff,(char*)uBuff,sizeof(uBuff));
		if(result==0){	
			printk("Data successfully written to kernel by write1 Process %d\n",kBuff);
			return SUCCESS;
		}
	}
	
*/	
}
