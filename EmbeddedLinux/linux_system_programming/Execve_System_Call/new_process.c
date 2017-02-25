/*
Author		: Sahil Khan
Date		: 25th Feb 2017
Description	: This is the new process that needs to be started using execve()
		  system call. It gets all the command line parameters and environ
		  variables and prints out on the console.
		  This new process has its own stack,heap and data section seperate 
		  from the process that initiated this process.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int j;
	
	for(j=0;j<argc;j++)
		printf("argv[%d] = %s\n",j,argv[j]);
	
	exit(EXIT_SUCCESS);
}
