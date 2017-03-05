/*
Author		: Sahil Khan
Date		: 4th March 2017
Description	: This lab covers the concepts of clean up handlers in threads. Here the
		  thread is either canceled or terminated normally depending whether we pass
		  any command line argument or not.
		  The cleanupHandler() will unlock the mutex held by the thread and also free
		  the memory allocated for the thread to work.These cleanup handlers are installed 
		  using pthread_cleanup_push() and pthread_cleanup_pop() functions which push the
		  address of each cleanup handlers in the corresponding stack of the thread.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

static pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int global_value;

static void cleanupHandler(void *arg)
{
	printf("Cleanup : freeing block %p\n",arg);
	free(arg);
	
	printf("Cleanup : unlocking mutex\n");
	if((pthread_mutex_unlock(&mtx)) != 0){
		printf("Error : pthread_mutex_unlock() failed\n");
		exit(EXIT_FAILURE);
	}
}

static void* threadFunc(void *arg)
{
	void *buf = NULL;
	buf = malloc(0x10000);
	printf("Thread : Allocated memory at %p \n",buf);
	
	if((pthread_mutex_lock(&mtx)) != 0){
		printf("Error : pthread_mutex_lock() failed\n");
		exit(EXIT_FAILURE);
	}
	
	pthread_cleanup_push(cleanupHandler,buf);
	
	while(global_value == 0){
		if((pthread_cond_wait(&cond,&mtx)) != 0){
			printf("Error : pthread_cond_wait() failed\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("thread : condition wait loop completed\n");
	pthread_cleanup_pop(1);
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
	
	sleep(2);
	
	if(argc == 1){
		printf("main : about to cancel thread\n");
		if((pthread_cancel(thrd1)) != 0){
			printf("Error : pthread_cancel() failed\n");
			exit(EXIT_FAILURE);
		}
	} else{
		printf("main : about to signal condition variable\n");
		global_value = 1;
		if((pthread_cond_signal(&cond)) != 0){
			printf("Error : pthread_cond_signal() failed\n");
			exit(EXIT_FAILURE);
		}
	}
			
	printf("Joining the Thread\n");
	if(pthread_join(thrd1,&res) != 0){
		printf("Error : pthread_join() failed\n");
		exit(EXIT_FAILURE);
	}	
	
	if(res == PTHREAD_CANCELED)
		printf("main : thread was canceled\n");
	else
		printf("main : thread was terminated normally\n");
	
	exit(EXIT_SUCCESS);
}
