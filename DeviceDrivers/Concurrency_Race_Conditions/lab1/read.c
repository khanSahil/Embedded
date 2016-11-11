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
	int fd,data,value,retval;
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("USERT_SPACE READ: ERROR fd open failed\n");
		return FAILURE;
	}	
		
	for(data = 1; data <= 20; data++){

		retval = read(fd,(char*)&value,sizeof(value));
		if(retval == SUCCESS){
			printf("USERT_SPACE READ: Data read successfully : %d\n",value);
		}
		else{
			printf("USER_SPACE READ: ERROR reading the data %d\n",value);		
		}
	}
	close(fd);
}




	

