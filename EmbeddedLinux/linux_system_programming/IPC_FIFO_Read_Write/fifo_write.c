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
#define TEN_MEG_MEM (1024 * 1024 * 10)

int main(int argc, char *argv[])
{
	int pipe_fd,res,bytes_sent=0,open_mode = O_WRONLY;
	char buffer[BUFF_SIZE+1];
	
	//strcpy(buffer,argv[1]);
	
	if(access(FIFO_NAME, F_OK) == -1){
		if(mkfifo(FIFO_NAME,0777) != 0){
			printf("Could not create fifo %s\n",FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}
	
	printf("Process %d opening FIFO O_WRONLY\n",getpid());
	pipe_fd = open(FIFO_NAME,open_mode);
	printf("Process %d result %d\n",getpid(),pipe_fd);
	
	if(pipe_fd != -1){
		while(bytes_sent < TEN_MEG_MEM){
			if((res=write(pipe_fd, buffer , BUFF_SIZE)) == -1){
				printf("Error : write() failed\n");
				exit(EXIT_FAILURE);
			}
			bytes_sent += res;
		}
		(void)close(pipe_fd);
	} else{
		exit(EXIT_FAILURE);
	}
	
	printf("Process %d finished\n",getpid());	
	exit(EXIT_SUCCESS);
}
