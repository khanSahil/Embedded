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
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <limits.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFF_SIZE PIPE_BUF

int main(int argc, char *argv[])
{
	int pipe_fd,res=0,bytes_read=0,open_mode = O_RDONLY;
	char buffer[BUFF_SIZE+1];
	
	memset(buffer,'\0',sizeof(buffer));
	
	printf("Process %d opening FIFO O_RDONLY\n",getpid());
	pipe_fd = open(FIFO_NAME,open_mode);
	printf("Process %d pipe_fd %d\n",getpid(),pipe_fd);
	
	if(pipe_fd != -1){
		do{
			res=read(pipe_fd, buffer , BUFF_SIZE);
			bytes_read +=res; 
		}while(res > 0);
		(void)close(pipe_fd);
	}else{
	exit(EXIT_FAILURE);
	}
	
	printf("Process %d finished %d bytes read\n",getpid(),bytes_read);	
	exit(EXIT_SUCCESS);
}
