/*
Author		: Sahil Khan
Date		: 13th Feb 2017
Description	: This lab covers the concepts of signal in linux operating systems.
		  This basic example demonstrates how a signal handler is installed for 
		  a particular signal event. I have used Ctrl+C event and installed a 
		  handler which prints the signal number in the handler that is called 
		  automatically whenever that event is generated.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#define TRUE 1

static void sig_handler(int sig_num)
{
	printf("\nOuch its signal : %d\n",sig_num);
}

int main(int argc, char *argv[])
{
	int count;
	
	if(signal(SIGINT,sig_handler) == SIG_ERR){
		printf("Signal hanlder installation failed\n");
		exit(EXIT_FAILURE);
	}
	
	while(TRUE){
		count++;
		printf("Count : %d\n",count);
		sleep(3);
	}
	exit(EXIT_SUCCESS);
}
