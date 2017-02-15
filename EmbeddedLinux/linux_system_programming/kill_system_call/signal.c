/*
Author		: Sahil Khan
Date		: 14th Feb 2017
Description	: This lab covers the concepts of kill system call in linux operating 
		  systems.This basic example demonstrates how kill signal can be send from
		  this process to any other process in the system.Signal number and PID value
		  is accepted through command line and various conditions were checked to ensure
		  this the process to be killed is present in the system or this running process
		  has the rights to kill that process. Corresponding error messages also printed 
		  in case any errror occurs during killing of the process. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
int main(int argc, char *argv[])
{
	int status, sig , pid;
	
	if(argc != 3 || strcmp(argv[1],"--help") == 0){
		printf("Please enter the signal  number and PID value\n");
		printf("Format : ./signal.c <signal_num> <PID>\n");
		exit(EXIT_FAILURE);
	}
	
	status = kill(atoi(argv[2]), atoi(argv[1]));
	if((sig !=0) && (status == -1)){
		printf("Kill Error\n");
		exit(EXIT_FAILURE);
	}
	else{
		if(status == 0)
			printf("Process exists and we can send it a signal\n");
		else{
			if(errno == EPERM)
				printf("Process exits but we don't have permission to send it a signal\n");
			else if(errno == ESRCH)
				printf("Process doesn't exist\n");
			else
				exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}
