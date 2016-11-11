#include<linux/ioctl.h>

#define MAGIC_NUM 's'
#define SET_DELAY 			_IOW(MAGIC_NUM,1,int)

