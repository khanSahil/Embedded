/*
Author		: Sahil Khan
Date		: 9th March 2017
Description	: This lab covers the concepts of pipes, and how you can run the shell 
		  command using popen() system call. It can be opened in read or write mode
		  dependin upon whether you are reading from the pipe created or write to
		  the pipe being created. 
		  In this example a pipe is created using popen() in a read mode, 
		  which executed the command "ls -l <path>" and the output is read into the 
		  buff[512] which finally gets printed on the stdout.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 512

int main(int argc, char *argv[])
{
	FILE *fd_in;
	char buff[BUF_SIZE];
	
	if((fd_in = popen("ls -l /tmp/","r")) == 0){
		printf("Error : popen() failed\n");
		exit(EXIT_FAILURE);
	}
	
	while(fgets(buff,BUF_SIZE,fd_in) > 0){
		printf("%s",buff);
	}
	pclose(fd_in);
	exit(EXIT_SUCCESS);
}
