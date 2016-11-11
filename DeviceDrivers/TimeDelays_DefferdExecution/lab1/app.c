#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "app.h"

#define SUCCESS 0
#define FAILURE -1

int main()
{

	int fd,delay = 5;	
	time_t now;
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("fd open failed\n");
		return FAILURE;
	}

	printf("USER_SPACE : Setting the Delay = %d\n",delay);
	
	time(&now);
	printf("%s", ctime(&now));
	
	ioctl(fd,SET_DELAY,&delay);
	
	time(&now);
	printf("%s", ctime(&now));
	
	close(fd);
}

