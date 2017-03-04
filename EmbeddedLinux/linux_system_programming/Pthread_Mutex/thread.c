/*
Author		: Sahil Khan
Date		: 3rd March 2017
Description	: This lab covers the concepts of threads synchronization using pthreads.
		  Two threads were created and each thread executing same thread function in
		  which the global variable is modified using a local thread function variable.
		  Critical section pertains to the code that is modifying the global variable
		  so we need to acquire the mutex lock and release it after it the gloabl_variable
		  is modified.
		  If another thread is trying to acquire the same lock, it will be blocked and will
		  get the lock, when prev owner of the lock releases it.This way we can assure none
		  of the threads are overwriting the value written by previous threads.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

static int global_value;
static pthread_mutex_t mtx_lock = PTHREAD_MUTEX_INITIALIZER;

static void* threadFunc(void *arg)
{
	int loops = *((int*) arg);
	int local_value,count,status;
	
	for(count = 0 ; count < loops ; count++){
		if(pthread_mutex_lock(&mtx_lock) != 0){
			printf("Error : mtx_lock was not acquired , Exiting\n");
			exit(EXIT_FAILURE);
		}
		local_value = global_value;
		local_value++;
		global_value = local_value;
		
		if(pthread_mutex_unlock(&mtx_lock) != 0){
			printf("Error : mtx_lock was not relased , Exiting\n");
			exit(EXIT_FAILURE);
		}
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thrd1,thrd2;
	int loop_count;
	
	loop_count = argc > 1 ? atoi(argv[1]) : 10000000;
	
	printf("Creating two Threads\n");
	if(pthread_create(&thrd1,NULL,threadFunc,&loop_count) != 0){
		printf("Error : pthread_create() failed for thread_1\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_create(&thrd2,NULL,threadFunc,&loop_count) != 0){
		printf("Error : pthread_create() failed for thread_2\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Joining the two Threads\n");
	if(pthread_join(thrd1,NULL) != 0){
		printf("Error : pthread_join() failed for thread_1\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_join(thrd2,NULL) != 0){
		printf("Error : pthread_join() failed for thread_2\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Global_Value %d\n",global_value);	
	exit(EXIT_SUCCESS);
}
