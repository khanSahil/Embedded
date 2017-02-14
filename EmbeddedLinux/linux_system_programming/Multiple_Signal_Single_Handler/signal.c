/*
Author		: Sahil Khan
Date		: 13th Feb 2017
Description	: This lab covers the concepts of signals in linux operating systems.
		  This example moves one step ahead and demonstrates how a single signal 
		  handler can be installed for multiple signal events. I have used Ctrl+C 
		  and Ctrl+Z events and installed a single handler sig_handler() which prints 
		  the signal number whenever these two events gets generated.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#define TRUE 1

static void sig_handler(int sig_num)
{	
	if(sig_num == SIGINT)
		printf("\nOuch signal %d caused Ctrl-C event\n",sig_num);
	else if (sig_num == SIGTSTP){
		printf("\nOuch signal %d caused Ctrl-Z event\n",sig_num);	
		exit(EXIT_SUCCESS);
	}	
}

int main(int argc, char *argv[])
{
	int count;
	
	if((signal(SIGINT,sig_handler) == SIG_ERR) || (signal(SIGTSTP,sig_handler) == SIG_ERR)){
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
