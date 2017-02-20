/*
Author		: Sahil Khan
Date		: 19th Feb 2017
Description	: This lab covers the concept related to multiple children created by parent
		  using fork() system call. Each child process is created and slept for different
		  amount of time. 
		  The parent process calls a wait() system call to get the PID value of the child
		  just exited and prints the respective information. Each argument in command line 
		  arguments is the sleep time for each process and total number of arguments passed 
		  tells the total number of child process going to be creatded.
		  The for loop continues for infinite times and parent check if there are any child
		  still present in the system, if not it also exits successfully.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc , char *argv[])
{
	pid_t pid_value;
	int numDead=0,j;
	
	if(argc < 2 || strcmp(argv[1],"--help") == 0){
		printf("Please enter command line arguments\n");
		printf("Usage : ./process <sleep_time1> <sleep_time2> ... <sleep_timeN>\n");
		exit(EXIT_SUCCESS);
	}
	
	setbuf(stdout,NULL);

	for(j = 1; j < argc ; j++)
	{
		
		switch(fork()){
		case -1:
			printf("Child process not created successfully\n");
			exit(EXIT_FAILURE);
		case 0:
			printf("Child %d started with PID %ld, sleeping %s seconds\n",j,(long)getpid(),argv[j]);
			sleep(atoi(argv[j]));
			_exit(EXIT_SUCCESS);
		default:
			break;
		}
	}
	
	for(;;){
		pid_value = wait(NULL);
		if(pid_value == -1){
			if(errno == ECHILD){
				printf("No more children - bye..!!\n");
				exit(EXIT_SUCCESS);
			}
			else{
				printf("Error with wait() system call\n");
				exit(EXIT_FAILURE);
			}
		}
				
		numDead++;
		printf("wait() returned child PID %ld (numDead = %d)\n",(long)pid_value,numDead);
	}
}
