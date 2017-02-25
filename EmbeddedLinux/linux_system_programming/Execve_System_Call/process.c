/*
Author		: Sahil Khan
Date		: 25th Feb 2017
Description	: This lab demonstrates the concept related to execve() system call.
		  The execve() system call is used to run a new process which has its
		  own heap , stack and data segments as well as text/code segments.We 
		  can pass command line arguments as well as environ variable using 
		  execve() system call. 
		  The new process will start from its own main function and gets all the
		  command line arguments and environ variable that was passed earlier.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char *argVec[10];
	
	if(argc !=2 || strcmp(argv[1],"--help") == 0){
		printf("Format : ./process ./new_process\n");
		exit(EXIT_FAILURE);
	}
	
	argVec[0] = strrchr(argv[1],'/');
	if(argVec[0] != NULL)
		argVec[0]++;
	argVec[1] = "hello new process";
	argVec[2] = "good bye new process";
	argVec[3] = NULL;
	
	execve(argv[1],argVec,NULL);
	printf("Error : execve() system call failed\n");
}
