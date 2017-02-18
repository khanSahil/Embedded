/*
Author		: Sahil Khan
Date		: 17th Feb 2017
Description	: This lab demonstrates how a child process is created and modifies
		  the data shared between parent and child process.Both local and global 
		  data is modified and gets reflected when parent process is scheduled.
		  Sleep() is called inside parent process just to make sure that child 
		  runs first.
		  In fork() system call , there is no gaurantee that child process runs
		  first or parent process runs first.So to make sure this parent sleep 
		  for 3 seconds and scheduler schedules child process and finally the 
		  parent process.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int globalData = 111;

int main()
{
	pid_t pid_value;
	int localData = 222;
	switch(pid_value = fork()){
	case -1:
		printf("Child process not created successfully\n");
		exit(EXIT_FAILURE);
	case 0:
		globalData *=2;
		localData *=2;
		break;
	default:
		sleep(3);
		break;
	}
	
	printf("PID = %ld (%s) globalData = %d localData = %d\n",
					(long)getpid(),pid_value == 0 ? "child" : "parent",
					globalData,localData);
					
	exit(EXIT_SUCCESS);
}
