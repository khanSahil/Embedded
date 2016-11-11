#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SUCCESS 0
#define FAILURE -1

int main(){

	int fd,data,retval;
	
	fd=open("/dev/CharacterDrive",O_RDWR,0666);
	if(fd<0){
		perror("USERT_SPACE WRITE-2: ERROR fd open failed\n");
		return FAILURE;
	}
	
	for(data = 51; data <= 60; data++){
	
		retval = write(fd,(char*)&data,sizeof(data));
		if(retval == SUCCESS){
			printf("USERT_SPACE WRITE-2: Data written successfully : %d\n",data);
		}
		else{
			printf("USER_SPACE WRITE-2: ERROR writing the data %d\n",data);		
		}
	}
	close(fd);
}

