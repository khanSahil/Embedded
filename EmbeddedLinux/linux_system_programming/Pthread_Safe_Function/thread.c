/*
Author		: Sahil Khan
Date		: 4th March 2017
Description	: This lab covers the concepts of thread safe functions used in pthreads.
		  Here perror() function is defined which is thread-safe as the value written 
		  into the buffer used by pthread is not overwritten by the created thread.
		  Here the buffer is not defined as global static, but created through malloc.
		  After executing this program you will see that the final value printed inside
		  the main thread has not been overwritten by the created thread.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

#define MAX_ERROR_LEN 256

static void destructor(void *buf)
{
	free(buf);
}
static void createKey(void)
{
	if((pthread_key_create(&strerrorKey,destructor)) !=0){
		printf("Error : pthread_key_create() function failed to create key\n");
		exit(EXIT_FAILURE);
	}
}

char* strerror(int err)
{
	char *buf;
	if((pthread_once(&once,createKey)) !=0){
		printf("Error : pthread_once() failed\n");
		exit(EXIT_FAILURE);
	}

	if((buf = pthread_getspecific(strerrorKey)) == NULL){
		if((buf = malloc(MAX_ERROR_LEN)) == NULL){
			printf("malloc() failed to creat Thread Specific Buffer\n");
			exit(EXIT_FAILURE);
		}
		if((pthread_setspecific(strerrorKey,buf)) !=0){
			printf("pthread_setspecific() function failed\n");
			exit(EXIT_FAILURE);
		}
	}
				
	if(err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL){
		snprintf(buf,MAX_ERROR_LEN,"Unknown Error %d\n",err);
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
