/*
Author		: Sahil Khan
Date		: 9th Feb 2017
Description	: This lab captures the basics of file handling. System calls are used to open
		  read, write and close files. This program reads source file and writes to destination ]
		  file and finally closing them all. Proper error are checked wherever is necessar and 
		  corresponding exit status was returned.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFF_SIZE 256


int main(int argc , char *argv[])
{
	int in_fd,out_fd,openFlags;
	mode_t filePerm;
	size_t numReads;
	char buff[BUFF_SIZE];
	
	if(argc != 3 ){
		printf("Usage : %s <source_file_1> <dest_file_2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if((in_fd = open(argv[1], O_RDONLY)) == -1){
		printf("Source File %s cannot be opened\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	
	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ;
	
	if((out_fd = open(argv[2],openFlags,filePerm)) == -1){
		printf("Destination File %s cannot be opened\n",argv[2]);
		exit(EXIT_FAILURE);
	}
	
	while((numReads = read(in_fd,buff,BUFF_SIZE)) > 0 ){
		if(write(out_fd,buff,numReads) != numReads){
			printf("Destination %s file cannot be written completely\n",argv[2]);
			exit(EXIT_FAILURE);
		}
	}
	
	if(numReads == -1){
		printf("Problem in reading input file %s \n",argv[1]);
		exit(EXIT_FAILURE);
	}
	
	if(close(in_fd) == 1){
		printf("Error : closing input file\n");
		exit(EXIT_FAILURE);
	}
	
	if(close(out_fd) == 1){
		printf("Error : closing output file\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
	
	
