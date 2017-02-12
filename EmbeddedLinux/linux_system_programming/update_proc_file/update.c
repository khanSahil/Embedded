/*
Author		: Sahil Khan
Date		: 9th Feb 2017
Description	: This lab covers the concepts of /proc/ file system and how you can edit some of the 
		  enteries in the /proc/ file system. Not all files are writable at normal user , so you
		  have to be privilege user to edit these files.
 		  This program accepts the new value for pid_max as a command line parameter and updates 
		  pid_max vlaue accordingly. If you don't provide any value it will simply show the old 
		  value of pid_max.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#define MAX_LINE 100


int main(int argc , char *argv[])
{
	int fd = 0;
	char line[MAX_LINE];
	ssize_t n;
	
	if((fd = open("/proc/sys/kernel/pid_max",(argc > 1) ? O_RDWR : O_RDONLY)) == -1){
		printf("Error opening the Proc file\n");
		exit(EXIT_FAILURE);
	}
	
	if((n = read(fd,line,MAX_LINE)) == -1){
		printf("Error reading pid_max\n");
		exit(EXIT_FAILURE);
	}
	
	if(argc > 1){
		printf("Old Value : %.*s", (int)n,line);
		if(write(fd,argv[1],strlen(argv[1])) != strlen(argv[1])){
			printf("Error : pid_max not updated\n");
			exit(EXIT_FAILURE);
		}
		
		system("echo /proc/sys/kernel/pid_max new value" " `cat /proc/sys/kernel/pid_max`");
	}
	else
		system("echo /proc/sys/kernel/pid_max old value" " `cat /proc/sys/kernel/pid_max`");
	exit(EXIT_SUCCESS);
}
	
	
