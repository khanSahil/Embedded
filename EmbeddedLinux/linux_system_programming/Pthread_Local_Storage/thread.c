/*
Author		: Sahil Khan
Date		: 4th March 2017
Description	: This lab covers the concepts of thread safe functions used in pthreads.
		  Here perror() function is defined which is thread-safe as the buf defined to
		  be used in perror() function is define with a keyword __thread which puts 
		  this buffer inside the local storage for each thread.
		  Whenever main thread calls perror() it uses its own buf[] and new thread
		  uses it own buf[] defined in their respective local storage area.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define MAX_ERROR_LEN 256

static __thread char buf[MAX_ERROR_LEN];

char* strerror(int err)
{
	if(err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL){
		snprintf(buf,MAX_ERROR_LEN,"Unknown Error %d",err);
	}else {
		strncpy(buf,_sys_errlist[err],MAX_ERROR_LEN-1);
		buf[MAX_ERROR_LEN-1] = '\0';
	}
	return buf;
}

static void* threadFunc(void *arg)
{
	char *s;
	printf("Other thread about to call strerror()\n");
	s=strerror(EPERM);
	printf("Other thread:	s (%p) = %s\n",s,s);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thrd1;
	char *s;
	s = strerror(EINVAL);
	printf("Main thread has called strerror()\n");
	
	if(pthread_create(&thrd1,NULL,threadFunc,NULL) != 0){
		printf("Error : pthread_create() failed\n");
		exit(EXIT_FAILURE);
	}
	
	if(pthread_join(thrd1,NULL) != 0){
		printf("Error : pthread_join() failed\n");
		exit(EXIT_FAILURE);
	}	
	printf("Main thread:	s (%p) = %s\n",s,s);
	exit(EXIT_SUCCESS);
}
