/*
Author		: Sahil Khan
Date		: 16th March 2017
Description	: This lab covers the concepts of system V shared memory. In this program  
		  we created a shared memory segment, in which we are writing the data,after 
		  reading it from the file , provided at command line.
		  This shared memory is available to reader also which reads it and writes it
		  to the file provided at the command line.
		  System V semaphores are used to synchronize the read and write between reader
		  and writer so that the data is not lost.
*/

#include "header.h"

int main(int argc, char *argv[])
{	
     char *map_start_addr;
     int fd;
     struct stat sb;
     
     if(argc !=2 || strcmp(argv[1],"--help") == 0)
     	pErr("%s <file>",argv[0]);
     
     if((fd = open(argv[1],O_RDONLY)) == -1)
     	pErr("open() failed");
     	
     if((fstat(fd,&sb)) == -1)
     	pErr("fstat() failed");
     	
     if((map_start_addr = mmap(NULL,sb.st_size,PROT_READ,MAP_PRIVATE,fd,0)) == MAP_FAILED)
     	pErr("mmap() failed");
     	
     if(write(STDOUT_FILENO,map_start_addr,sb.st_size) != sb.st_size)
     	pErr("write() failed");
     	
     if(munmap(map_start_addr,sb.st_size) == -1)
     	pErr("munmap() failed");
     exit(EXIT_SUCCESS);
     
}

