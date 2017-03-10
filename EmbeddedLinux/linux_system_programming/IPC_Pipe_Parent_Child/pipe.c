/*
Author		: Sahil Khan
Date		: 8th March 2017
Description	: This lab covers the concepts of pipes, one of the way through which 
		  processes can communicate with each other. Here parent process creates 
		  a pipe and forks to create child which inherits all the descriptors from
		  the parent process including that of pipe.
		  The parent process then writes to the write end (pipe[1]) of the pipe and
		  the child reads from the read end (pipe[0]) of the pipe,the message that
		  we pass as command line parameter.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 10

int main(int argc, char *argv[])
{
		int p_fd[2];
		char buf[BUF_SIZE];
		ssize_t numRead;
		
		if(argc !=2 || strcmp(argv[1],"--help") == 0){
			printf("Usage : ./pipe <'message_string'>\n");
			exit(EXIT_FAILURE);
		}
		
		if(pipe(p_fd) == -1){
			printf("Line:%d-Error : pipe() failed\n",__LINE__);
			exit(EXIT_FAILURE);
		}
		
		switch(fork()){
		case -1:
			printf("Line:%d-Error : fork() failed\n",__LINE__);
			exit(EXIT_FAILURE);
		case 0:
			if(close(p_fd[1])==-1){
				printf("Line:%d-Error : close(p_fd[1]) failed\n",__LINE__);
				exit(EXIT_FAILURE);
			}
			
			for(;;)
			{
				numRead = read(p_fd[0],buf,BUF_SIZE);
				if(numRead == -1){
					printf("Line:%d-Error : read() failed\n",__LINE__);
					exit(EXIT_FAILURE);
				}
				if(numRead == 0){
					break;
				}
				if(write(STDOUT_FILENO,buf,numRead) != numRead){
					printf("Line:%d-Error : write() failed\n",__LINE__);
					exit(EXIT_FAILURE);
				}
			}
			write(STDOUT_FILENO,"\n",1);
			if(close(p_fd[0]) == -1){
				printf("Line:%d-Error : close(p_fd[0]) failed\n",__LINE__);
				exit(EXIT_FAILURE);
			}
			_exit(EXIT_SUCCESS);
		default:
			if(close(p_fd[0])==-1){
				printf("Line:%d-Error : close(p_fd[0]) failed\n",__LINE__);
				exit(EXIT_FAILURE);
			}
			
			if(write(p_fd[1],argv[1],strlen(argv[1])) != strlen(argv[1])){
				printf("Line:%d-Error : write failed\n",__LINE__);
				exit(EXIT_FAILURE);
			}
			
			if(close(p_fd[1]) == -1){
				printf("Line:%d-Error : close(p_fd[1]) failed\n",__LINE__);
				exit(EXIT_FAILURE);
			}
			
			wait(NULL);
			exit(EXIT_SUCCESS);
		}
}
