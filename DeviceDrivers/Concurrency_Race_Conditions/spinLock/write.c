#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUCCESS 0
#define FAILURE -1

int main()
{

	int fd;
	char mbuff[]="Hi my name is Sahil Khan\n";
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("fd open failed\n");
		return FAILURE;
	}

	write(fd,mbuff,sizeof(mbuff));
	close(fd);
}

