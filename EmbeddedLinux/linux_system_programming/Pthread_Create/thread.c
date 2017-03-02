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
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

static void* threadFunc(void *arg)
{
	char *s = (char*)arg;
	printf("ThreadFunc : %s",s);
	return (void*)strlen(s);
}

int main(int argc, char *argv[])
{
	pthread_t thrd1;
	void *res;
	
	if(pthread_create(&thrd1,NULL,threadFunc,"Hello Thread\n") != 0){
		printf("Error : pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Joining the Thread\n");
	if(pthread_join(thrd1,&res) != 0){
		printf("Error : pthread_join() failed\n");
		exit(EXIT_FAILURE);
	}	
	
	printf("Thread returned %ld\n",(long)res);
	exit(EXIT_SUCCESS);
}
