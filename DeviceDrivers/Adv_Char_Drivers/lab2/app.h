#include<linux/ioctl.h>

struct deviceDescriptor{

	char deviceName[20];
	int deviceID;
	int productID;
};

#define MAGIC_NUM 's'
#define SET_BAUD 			_IOW(MAGIC_NUM,1,int)
#define SET_PARITY 			_IO(MAGIC_NUM,2) 
#define SET_STOP_BITS 			_IO(MAGIC_NUM,3)
#define FLUSH_BUFFERS 			_IO(MAGIC_NUM,4)
#define READ_DEVICE_CONFIGURATION 	_IOR(MAGIC_NUM,5,struct deviceDescriptor*)

