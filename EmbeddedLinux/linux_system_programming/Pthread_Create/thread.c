/*
Author		: Sahil Khan
Date		: 1st March 2017
Description	: This lab covers the concepts of threads in very basic way.In this
		  lab a thread ic created, argument is passed to the define thread function
		  and finally received the return value from the thread function.
		  Very basic and easy way of showing how threads are created and joined when
		  they get terminated or returned from the thread function.
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
