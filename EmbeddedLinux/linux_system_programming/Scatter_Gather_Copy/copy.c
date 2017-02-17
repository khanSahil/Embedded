/*
Author		: Sahil Khan
Date		: 10th Feb 2017
Description	: This lab captures one of the interesting system calls that linux system provides us,
		  it is called as readv(). This system call does scatter input and is used to read a file,
		  places the data in different buffer as specified by user. Each of the buffers starting 
		  the one defined by iov[0], is completely filled before readv() proceeds to the next buffer. 
		  Corresponding to readv() we also have writev() which does scatter output. 
		  The three buffers I have used is str1,str2,str3 which is of char type, you can use anytype
		  of buffer like struct , char etc. The file read is hello.txt which is of 324 bytes, thats why
		  each buffer is of size 108 bytes just to print all the read data at the end with loosing any.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <string.h>
#include <errno.h>
#define BUFF_SIZE 256


int main(int argc , char *argv[])
{
	int fd;
	struct iovec iov[3];
	#define STR_SIZE 108
	
	char str1[STR_SIZE];	// first buffer
	char str2[STR_SIZE];	// second buffer	
	char str3[STR_SIZE];	// third buffer
	
	size_t numRead = 0,totRequired = 0;
	
	if(argc != 2){
		printf("Error : Usage ./copy <sourec_file_name>\n");
		exit(EXIT_FAILURE);
	}
	
	if((fd = open(argv[1],O_RDONLY)) == -1){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	iov[0].iov_base = str1;
	iov[0].iov_len= STR_SIZE;
	totRequired += iov[0].iov_len;
	
	iov[1].iov_base = str2;
	iov[1].iov_len= STR_SIZE;
	totRequired += iov[1].iov_len;
	
	iov[2].iov_base = str3;
	iov[2].iov_len= STR_SIZE;
	totRequired += iov[2].iov_len;
	
	
	if((numRead=readv(fd,iov,3)) == -1){
		perror("readv");
		exit(EXIT_FAILURE);
	}
	else if(numRead < totRequired)
		printf("Read fewer bytes than requested\n");
	
	printf("\nFirst Buffer : \n");
	printf("%s\n",str1);
	
	printf("\nSecond Buffer : \n");
	printf("%s\n",str2);
	
	printf("\nThird Buffer : \n");
	printf("%s\n",str3);
		
	printf("Total bytes requested : %ld; bytes read : %ld\n",(long)totRequired,(long)numRead);
	exit(EXIT_SUCCESS);
}
	
	
