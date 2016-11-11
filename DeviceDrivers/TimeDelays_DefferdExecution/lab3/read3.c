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
	int fd,data,value = 10,retval;
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("USERT_SPACE READ: ERROR fd open failed\n");
		return FAILURE;
	}	
	
	retval = read(fd,(char*)&value,sizeof(value));
		
	close(fd);
}




	

