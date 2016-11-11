#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include <linux/ioctl.h>
#include<asm/ioctl.h>
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
.read=dev_read,
.write=dev_write,
.unlocked_ioctl= dev_ioctl,
.release=dev_release,
};

dev_t deviceno;
struct cdev *charDrive;

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
	char kBuff[]="My name is Sahil Khan and I am sending the data to User Space\n";
	ssize_t retval;
	int result;
	result=copy_to_user(uBuff,kBuff,count);
	if(result==0)
	{
		printk("Data successfully written to user space with bytes--%d\n",sizeof(kBuff));
		*ofs += sizeof(uBuff);
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
	char kBuff[50];
	ssize_t retval;
	int result;
	result=copy_from_user(kBuff,uBuff,count);
	if(result==0)
	{
		printk("Data successfully written to kernel space with bytes--%d\n",sizeof(uBuff));
		printk("%s\n",kBuff);
		*ofs += count;
		return sizeof(uBuff);
	}
	else
	{
		printk("Data copy failed\n");
		return -EFAULT;
	}
}

int dev_ioctl(struct file *filp,unsigned int cmd,unsigned long int arg)
{
	int baud,retValue = 10;
	struct deviceDescriptor dC;
	strcpy(dC.deviceName,"CharacterDrive\n");
	dC.productID = 129;
	dC.deviceID = 12;
	
	switch(cmd)
	{
		case SET_BAUD:
			printk("Setting the Baud Rate : %d\n",*(int*)arg);
			retValue = get_user(baud,(int *)arg);
			if(retValue == SUCCESS){
				printk("Recievd the Baud Successfully %d\n",baud);
			}
			else{
				printk("ERROR: Baud not recieved correctly\n");
			}
		break;
		case SET_PARITY:
			printk("Setting the Parity\n");
		break;
		case SET_STOP_BITS:
			printk("Setting the Stop Bit\n");
		break;
		case FLUSH_BUFFERS:
			printk("Flushing the Buffers\n");
		break;
		case READ_DEVICE_CONFIGURATION:
			printk("Setting the Device Confiuration\n");
			retValue = copy_to_user((struct deviceDescriptor *)arg,&dC,sizeof(dC));
			printk("KERNEL_SPACE : ProductID %d\n",dC.productID);
			printk("KERNEL_SPACE : DeviceID %d\n",dC.deviceID);
			printk("KERNEL_SPACE : DeviceName %s\n",dC.deviceName);
			if(retValue == SUCCESS){
				printk("Sent the Device Configuration Succefully\n");
			}
			else{
				printk("ERROR: Device Conviguration not sent successfully\n");
			}
		break;
		default:
			printk("Invalid Command\n");
			return -1;
	}
	return SUCCESS;	
}
