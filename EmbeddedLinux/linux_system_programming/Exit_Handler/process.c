/*
Author		: Sahil Khan
Date		: 19th Feb 2017
Description	: This lab demonstrates how we can have customised exit function handlers in
		  Linux system. We can register different exit handlers using on_exit() and atexit()
		  function calls. All the exit handlers are called in the reverse order of  their 
		  registration. Here I have registered 4 handlers two from each on_exit() and atexit()
		  functions.
		  Whatever argument you pass in the final exit call , exit(2) in this case, will be 
		  used on_exit() function handlers and will be displayed exitStatus at the output. 
*/

#define _BSD_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>


static void atExitFunction1()
{
	printf("atexit() function called 1\n");
}

static void atExitFunction2()
{
	printf("atexit() function called 2\n");
}

static void onExitFunction(int exitStatus, void *arg)
{
	printf("on_exit() function called : status=%d, arg=%ld\n",exitStatus,(long)arg);
}

int main()
{
	if(on_exit(onExitFunction,(void*)10) != 0){
		printf("Error : onExitFunction\n");
	}
	if(atexit(atExitFunction1) != 0){
		printf("Error : atExitFunction1\n");
	}
	if(atexit(atExitFunction2) != 0){
		printf("Error : atExitFunction2\n");
	}
	if(on_exit(onExitFunction,(void*)20) != 0){
		printf("Error : onExitFunction\n");
	}
	exit(2);
}
