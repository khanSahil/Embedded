/*
Author		: Sahil Khan
Date		: 1st March 2017
Description	: This lab covers the concepts of threads cancellation. A thread is 
		  created and after 3 seconds the thread was cancelled and the joined by
		  the main thread in order to avoid making zombie thread.
		  When the thread is cancelled in this way, thread_join() returns a specific
		  value: THREAD_CANCELED and this is checked to make sure the thread was
		  cancelled successfully.
		  In this example no cancellation point was established, so the thread could
		  be cancelled when it is executing any instruction inside for loop.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

static void* threadFunc(void *arg)
{
	int count = 0;
	printf("New thread started\n");
	for(;;){
		printf("Count : %d\n",++count);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thrd1;
	void *res;
	
	if(pthread_create(&thrd1,NULL,threadFunc,"Hello Thread\n") != 0){
		printf("Error : pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}
	
	sleep(3);
	
	if(pthread_cancel(thrd1) != 0){
		printf("Error : pthread_cancel() failed\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Joining the Thread\n");
	if(pthread_join(thrd1,&res) != 0){
		printf("Error : pthread_join() failed\n");
		exit(EXIT_FAILURE);
	}	
	
	if(res == PTHREAD_CANCELED)
		printf("Thread cancelled successfully, returned %ld\n",(long)res);
	else
		printf("Error : thread_cancel() failed, retuned %ld\n",(long)res);
	exit(EXIT_SUCCESS);
}
