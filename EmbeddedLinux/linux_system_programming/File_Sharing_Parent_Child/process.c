/*
Author		: Sahil Khan
Date		: 17th Feb 2017
Description	: This lab covers how a parent process and child proceses shares the 
		  resources initially used by parent only before calling fork().In this
		  lab I am creating a temporary file whose file descriptor is shared 
		  between parent and child process. Child process using the same file 
		  descriptor changes the offset within the file as well as flags and this
		  modification is further verified inside the parent process.
		  In parent process I am calling wait() system calls that makes sure that 
		  child is executed first and there is no race condition so that parent
		  always gets the modificaion did by child.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>

int main()
{
	pid_t pid_value;
	int temp_fd,flags,status;
	char temp_file[]="/tmp/tempXXXXXX";
	
	setbuf(stdout,NULL);
	
	if((temp_fd = mkstemp(temp_file)) == -1){
		printf("Temporary file not created\n");
		exit(EXIT_FAILURE);
	}
	
	printf("File ofset before fork() : %lld\n",(long long)lseek(temp_fd,0,SEEK_CUR));
	if((flags = fcntl(temp_fd,F_GETFL)) == -1){
		printf("Couldn't get the flags status\n");
		exit(EXIT_FAILURE);
	}
	
	printf("O_APPEND flag before fork() is :%s\n",(flags & O_APPEND) ? "on" : "off");
	
	switch(pid_value = fork()){
	case -1:
		printf("Child process not created successfully\n");
		exit(EXIT_FAILURE);
	case 0:
		printf("Inside Child Process\n");
		if(lseek(temp_fd,1000,SEEK_SET) == -1){
			printf("SEEK_SET failed\n");
			exit(EXIT_FAILURE);
		}
		
		if((flags = fcntl(temp_fd,F_GETFL)) == -1){
			printf("Couldn't get the flags status\n");
			exit(EXIT_FAILURE);
		}
		
		flags |=O_APPEND;
		if(fcntl(temp_fd,F_SETFL,flags) == -1){
			printf("O_APPEND not set\n");
			exit(EXIT_FAILURE);
		}
	default:
		printf("Inside Parent process\n");
		if(wait(0) == -1){
			perror("wait");
			exit(EXIT_FAILURE);
		}
		printf("Child process has exited\n");
		
		printf("File offset in parent is : %lld\n",(long long)lseek(temp_fd,0,SEEK_CUR));
		if((flags = fcntl(temp_fd,F_GETFL)) == -1){
			printf("Couldn't get the flags status\n");
			exit(EXIT_FAILURE);
		}
			
		printf("O_APPEND flag in parent is : %s\n",(O_APPEND & flags) ? "on" : "off");
		exit(EXIT_SUCCESS);
	}
}
