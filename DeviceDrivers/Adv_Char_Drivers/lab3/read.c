#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUCCESS 0
#define FAILURE -1

int main()
{
	
	char lBuff[100];
	int fd;
	memset(lBuff,0,sizeof(lBuff));	
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("fd open failed\n");
		return FAILURE;
	}
	
	read(fd,lBuff,sizeof(lBuff));
	printf("USER_SPACE : %s\n",lBuff);
	close(fd);
}

