/*
Author		: Sahil Khan
Date		: 18th March 2017
Description	: This lab covers the concepts of system V memory mapping scheme. In this  
		  lab I have created a shared memory map which can be shared among different 
		  processes. The protection used here is both PROT_READ and PROT_WRITE.
		  This program reads command line arguments, if both file and data are there
		  in command line then the data is written to the starting address of the memory
		  and finally the data is also written to the file.
		  If only file name is mentioned then , the last stored data at the starting
		  address of the memory is displayed.
		  
		  Note : type the following command before excuting the file otherwise the bus error
		  will come and the core file will be generated: 
		  
		  dd if=/dev/zero of=s.txt bs=1 count=1024
		  
		  What this command will do is , it will create a file of size 1024 , filled with NULL
		  characters. This program when specify the file as the command line parameter , it has
		  to have non-zero size, otherwise core dump will be generated.
		  
		  
*/

#include "header.h"

int main(int argc, char *argv[])
{	
     char *map_start_addr = NULL;
     int fd;
     
     if(argc < 1 || strcmp(argv[1],"--help") == 0)
     	pErr("%s <file> [new_value]",argv[0]);
     
     if((fd = open(argv[1],O_RDWR)) == -1)
     	pErr("open() failed");
     	
     if((map_start_addr = mmap(NULL,MEM_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0)) == MAP_FAILED)
     	pErr("mmap() failed");
     
     printf("Current String = %.*s\n",MEM_SIZE,map_start_addr);
     
     if(argc > 2){
     	if(strlen(argv[2]) >= MEM_SIZE)
     		pErr("String length is more than the mapped shared memory region");
     		
     	memset(map_start_addr,0,MEM_SIZE);
     	strncpy(map_start_addr,argv[2],MEM_SIZE-1);
     	
     	if(msync(map_start_addr,MEM_SIZE,MS_SYNC) == -1)
     		pErr("msync() failed");
     		
     	
     	pDbg("Copied \"%s\" to shared memory",argv[2]);
     	
     	if(close(fd) == -1)
     		pErr("close() failed");
     }
     	
     if(munmap(map_start_addr,MEM_SIZE) == -1)
     	pErr("munmap() failed");
     exit(EXIT_SUCCESS);
     
}

