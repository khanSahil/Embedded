/*
Author		: Sahil Khan
Date		: 18th March 2017
Description	: This lab covers the concepts of system V memory mapping is discussed.Here
		  I have created anonymous memory mapping and then created a child using fork().
		  This child process inherits the mapping created inside the parent process.
		  Using this technique , data was shared between parent and child. Child process
		  modifies the value at the start address of the mapping and then parent process
		  that waits for the child process to complete, reads the modified value.
		  When both parent and child process finishes their respective tasks , they unmap
		  their corresping mapped region.
		  In this was Interprocess Communication can be done between parent and child.
*/

#include "header.h"

int main(int argc, char *argv[])
{	
	int *start_addr;
	if((start_addr = mmap(NULL,sizeof(int),PROT_READ| PROT_WRITE,MAP_SHARED| MAP_ANONYMOUS,-1,0)) == MAP_FAILED)
		pErr("Anonymous mmap() failed");
	
	*start_addr = 1;
	
	switch(fork()){
	case -1: 
		pErr("fork() failed");
	case 0:
		pDbg("Child started value : %d,",*start_addr);
		(*start_addr)++;
		
		if(munmap(start_addr,sizeof(int)) == -1)
			pErr("child : munmap() failed");
		exit(EXIT_SUCCESS);
	default :
		pDbg("Waiting for child to finish");
		if(wait(NULL) == -1)
			pErr("wait() failed");
		
		pDbg("In parent : value : %d",*start_addr);
		
		if(munmap(start_addr,sizeof(int)) == -1)
			pErr("parent : munmap() failed");
		exit(EXIT_SUCCESS);
	} 
}

