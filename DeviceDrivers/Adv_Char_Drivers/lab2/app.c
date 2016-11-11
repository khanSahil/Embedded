#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "app.h"

#define SUCCESS 0
#define FAILURE -1

int main()
{

	int fd,baud = 25;
	struct deviceDescriptor dC;
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("fd open failed\n");
		return FAILURE;
	}

	printf("USER_SPACE : Setting the Baud rate = %d\n",baud);
	ioctl(fd,SET_BAUD,&baud);
	ioctl(fd,SET_PARITY);
	ioctl(fd,SET_STOP_BITS);
	ioctl(fd,FLUSH_BUFFERS);
	ioctl(fd,READ_DEVICE_CONFIGURATION,&dC);
	printf("USER_SPACE : Device Configuration received\n");
	printf("USER_SPACE : Device ID = %d\n",dC.deviceID);
	printf("USER_SPACE : Product ID = %d\n",dC.productID);
	printf("USER_SPACE : Device Name = %s\n",dC.deviceName);
	close(fd);
}

