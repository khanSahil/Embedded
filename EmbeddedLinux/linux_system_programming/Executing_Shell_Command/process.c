/*
Author		: Sahil Khan
Date		: 26th Feb 2017
Description	: This lab covers the concept related to system() system call and how it
		  is used to execute any command. You can run any script or any other command
		  using system() call. This program loops inside and asks user to enter the
		  command that he/she wants to execute. 
		  System() call is equivalent to fork() exec() wait () exit() system calls
		  and gives us advantage of not using four different system calls rather than 
		  only single system call suffice.from fork() system call. 
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>

#define MAX_CMD_LEN 200

int main(int argc, char *argv[])
{
	char command[MAX_CMD_LEN];
	int status;
	
	for(;;)
	{
		printf("Enter the command : ");
		fflush(stdout);
		if(fgets(command,MAX_CMD_LEN,stdin) == NULL){
			printf("No Command entered , Exiting\n");
			exit(EXIT_FAILURE);
		}
		
		if(status = system(command) == -1){
			printf("Status returned -1 , exiting\n");
			exit(EXIT_FAILURE);
		}
		printf("System() returned : status = 0x%04x (%d,%d)\n",(unsigned int)status,status>>8,status&0xff);
	}
		exit(EXIT_SUCCESS);
}
