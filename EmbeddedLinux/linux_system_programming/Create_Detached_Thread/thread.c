/*
Author		: Sahil Khan
Date		: 1st March 2017
Description	: This lab covers another important concepts of threads.Here I have created
		  a thread that is by default detached from the main thread. This means that main 
		  thread doesn't have to call pthread_join() to get the status and value returned
		  from the threadFunc() function.
		  However there is a race condition here, if I put sleep(1) then it will allow to
		  threadFunc() function to execute completely and you will see the output from the 
		  threadFunc(). If I remove that sleep(1), then main() thread completes its execution
		  even before threadFunc() executed completely. So you won't see output from the
		  threadFunc() function.
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
	pthread_attr_t attr;
	void *res;
	
	if(pthread_attr_init(&attr) != 0 ){
		printf("Error : pthread_attr_init() failed\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED) != 0 ){
		printf("Error : pthread_attr_setdetachsatate() failed\n");
		exit(EXIT_FAILURE);
	}
		
	if(pthread_create(&thrd1,NULL,threadFunc,"Hello Thread\n") != 0){
		printf("Error : pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_attr_destroy(&attr) != 0 ){
		printf("Error : pthread_attr_destroy() failed\n");
		exit(EXIT_FAILURE);
	}
	
	sleep(1);
	exit(EXIT_SUCCESS);
}
