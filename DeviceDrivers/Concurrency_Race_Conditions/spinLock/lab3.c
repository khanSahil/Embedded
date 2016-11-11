#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include <linux/ioctl.h>
#include<asm/ioctl.h>
#include<linux/wait.h>
#include<linux/spinlock.h>
#define SUCCESS 0

MODULE_LICENSE("GPL");

int	dev_open(struct inode *, struct file *);
int dev_release(struct inode *, struct file *);
ssize_t dev_read(struct file *, char __user *, size_t,loff_t *);
ssize_t dev_write(struct file *, char __user *, size_t,loff_t *);

struct file_operations fops={
.owner=THIS_MODULE,
.open=dev_open,
.read=dev_read,
.write=dev_write,
.release=dev_release,
};

dev_t deviceno;
struct cdev *charDrive;
spinlock_t myLock;

static int flag = 0;
char kBuff[50];

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
	spin_lock_init(&myLock);
						
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
	int result;
	spin_lock(&myLock);
	result=copy_to_user(uBuff,kBuff,count);
	if(result==0)
	{
		printk("Data successfully written to user space with bytes--%d\n",sizeof(kBuff));
		*ofs += sizeof(uBuff);
		flag = 0;
		spin_unlock(&myLock);
		return sizeof(kBuff);
	}
	else
	{
		printk("Data copy failed\n");
		return -EFAULT;
	}
	
}

ssize_t dev_write(struct file *fp,char *uBuff,size_t count,loff_t *ofs)
{
	ssize_t retval;
	int result;
	spin_lock(&myLock);
	result=copy_from_user(kBuff,uBuff,count);
	if(result==0)
	{
		printk("Data successfully written to kernel space with bytes--%d\n",sizeof(uBuff));
		printk("%s\n",kBuff);
		*ofs += count;
		flag = 1;
		spin_unlock(&myLock);
		return sizeof(uBuff);
	}
	else
	{
		printk("Data copy failed\n");
		return -EFAULT;
	}
}
