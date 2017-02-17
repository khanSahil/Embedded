/*
Author		: Sahil Khan
Date		: 16th Feb 2017
Description	: This lab covers the concepts of how you can attach an alterante stack 
		  for a particular signal. Here I have attached alternate stack for signal 
		  SIGSEGV which is Invalid Memory Segment Access(ANSI). I have called a 
		  recursive function which used up the currently allocated stack and the
		  moment the process tries to access an invalid memory , SIGSEGV is fired 
		  off and as there is no spcae in the current stack for this signal, it 
		  uses the alternate stack.
		  I have printed out the address of the Top of Stack in each scenario to 
		  make it clear when process is using which stack.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <setjmp.h>

static void sig_handler(int sig_num)
{	
	int x;
	printf("Caught signal %d (%s)\n",sig_num,strsignal(sig_num));
	printf("Top of the handler stack near %10p\n",(void*)&x);
	fflush(NULL);
	_exit(EXIT_FAILURE);
}

static void overFlow(int call_count)
{
	char array[100000];
	printf("Call %4d - top of the stack near %10p\n",call_count,&array[0]);
	overFlow(call_count+1);
}

int main(int argc, char *argv[])
{
	stack_t sigstack;
	struct sigaction sa;
	int j;
	printf("Top of the standar stack near %10p\n",(void*)&j);
	
	if((sigstack.ss_sp = malloc(SIGSTKSZ)) == NULL){
		printf("Error : Allocating alternate stack to process\n");
		exit(EXIT_FAILURE);
	}
	
	sigstack.ss_size = SIGSTKSZ;
	sigstack.ss_flags = 0;
	
	if((sigaltstack(&sigstack,NULL)) == -1){
		printf("Stack cannot be linked to signal\n");
		exit(EXIT_FAILURE);
	}
	
	printf("Alternate stack is at	%10p-%p\n",sigstack.ss_sp,(char*)sbrk(0)-1);
	
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_ONSTACK;
	if(sigaction(SIGSEGV,&sa,NULL) == 1){
		printf("New signal handler cannot be installed for SIGSEGV signal\n");
		exit(EXIT_FAILURE);
	}
	
	overFlow(1);
}
