#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>



int main()
{

	int fd;
	char lBuff[100];
	//char mbuff[50];
	//memset(mbuff,0,sizeof(mbuff));
	char mbuff[]="Hi my name is Sahil Khan\n";
	memset(lBuff,0,sizeof(lBuff));
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0)
		perror("fd open failed\n");
		read(fd,lBuff,sizeof(lBuff));
		printf("data from kernal__%s\n",lBuff);
		write(fd,mbuff,sizeof(mbuff));
		close(fd);
}
