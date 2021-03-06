/*
Author		: Sahil Khan
Date		: 13th Feb 2017
Description	: This lab covers the concepts of masking and unmasking of signals in 
		  linux operating systems. I have used the same old code base but with slight
		  modification. I blocked the SIGINT signal and goes to sleep for 10 seconds.
		  In these 10 seconds if you type Ctrl+C from the console then the respective
		  handler won't be called as expected only Ctrl+Z works at this time. The moment 
		  it comes out of sleep both signals generated by Ctrl+C and Ctrl+Z start working 
		  together.Before blocking any signal we should always save the prev mask status as I 
		  have done so as to revert back to old mask in case necessary.
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
	sigset_t blockSet, prevMask;
	
	if((signal(SIGINT,sig_handler) == SIG_ERR) || (signal(SIGTSTP,sig_handler) == SIG_ERR)){
		printf("Signal hanlder installation failed\n");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&blockSet);
	sigaddset(&blockSet,SIGINT);
	
	if(sigprocmask(SIG_BLOCK,&blockSet,&prevMask) == -1){
		printf("Error : Specified signal cannot be masked\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Sleeping after masking the SIGINT signal\n");
	sleep(10);
	
	printf("Resetting the signal Mask for SIGINT\n");
	if(sigprocmask(SIG_SETMASK, &prevMask,NULL) == -1){
		printf("SIGINT signal cannot be unmasked\n");
		exit(EXIT_FAILURE);
	}
	while(TRUE){
		count++;
		printf("Count : %d\n",count);
		sleep(3);
	}
	exit(EXIT_SUCCESS);
}
