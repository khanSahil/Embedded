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
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0)
		perror("fd open failed\n");
	close(fd);
}
