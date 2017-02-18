/*
Author		: Sahil Khan
Date		: 16th Feb 2017
Description	: This lab covers the concept related to vfrok() and how it is different
		  from fork() system call.In fork() there is no gaurantee that child will run 
		  first or parent will run first,but in vfork() it is gauranteed that child is
		  going to run first. Even though we sleep inside child process, scheduler doesn't
		  scedules parent process.
		  Two more aspects related to vfork() is child process can modify its own file 
		  descriptors only without effecting parent file descriptors but any other data 
		  shared beteen parent child is shared and when child process modifies that the
		  parent gets the modified data(local_data in this case) when it is scheduled by scheduler
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

int main()
{
	pid_t pid_value;
	int local_data = 222;
	printf("local_data before vfork() : %d\n",local_data);
	switch(vfork()){
	case -1:
		printf("Child process not created successfully\n");
		exit(EXIT_FAILURE);
	case 0:
		write(STDOUT_FILENO,"Child Sleeping\n",16);
		sleep(3);
		write(STDOUT_FILENO,"Child Executing\n",16);
		local_data *=2;
		_exit(EXIT_SUCCESS);
		break;
	default:
		write(STDOUT_FILENO,"Parent Executing\n",17);
		printf("local_data : %d\n",local_data);
		exit(EXIT_SUCCESS);
	}
}
