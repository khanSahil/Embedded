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
int kBuff;
int pNum;

struct semaphore wR1,wR2,rD;

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
	
	sema_init(&wR1,1);
	sema_init(&wR2,0);
	sema_init(&rD,0);
						
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
	
	down(&rD);
	result=copy_to_user((char*)uBuff,(char*)&kBuff,sizeof(kBuff));
	if(result==0)
	{
		printk("Data successfully written to user space with bytes--%d\n",kBuff);
		if(pNum == 1)
			up(&wR2);
		else
			up(&wR1);
		return SUCCESS;
	}
	
	printk("Data copy failed\n");
	return -EFAULT;
}

ssize_t dev_write(struct file *fp,char *uBuff,size_t count,loff_t *ofs)
{
	ssize_t retval;
	int result;
	
	if(strcmp(current->comm,"write1")==0)
	{
		down(&wR1);
		result=copy_from_user((char*)&kBuff,(char*)uBuff,sizeof(uBuff));
		if(result==0)
		{	
			printk("Data successfully written to kernel by write1 Process %d\n",kBuff);
			pNum = 1;
			up(&rD);
			return SUCCESS;
		}
	}
	
	if(strcmp(current->comm,"write2")==0)
	{
		down(&wR2);
		result=copy_from_user((char*)&kBuff,(char*)uBuff,sizeof(uBuff));
		if(result==0)
		{	
			printk("Data successfully written to kernel by write2 Process %d\n",kBuff);
			pNum = 2;
			up(&rD);
			return SUCCESS;
		}
	}
	
	printk("Data write failed\n");
	return -EFAULT;
}
