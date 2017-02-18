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

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;


static void realtime_sig_handler(int sig_num, siginfo_t *si, void *ucontext)
{	
	if(sig_num == SIGINT || sig_num == SIGTERM){
		allDone = 1;
		return;
	}
	
	sigCnt++;
	printf("Caught Signal %d\n",sig_num);
	printf("si_signo=%d, si_code=%d (%s),",si->si_signo,si->si_code,
									(si->si_code == SI_USER)?"SI_USER":
									(si->si_code == SI_QUEUE)?"SI_QUEUE":"other");
	
	printf("si_value=%d\n",si->si_value.sival_int);
	printf("	si_pid=%ld, si_uid=%ld\n",(long)si->si_pid,(long)si->si_uid);
	sleep(handlerSleepTime);
}


int main(int argc, char *argv[])
{
	int sig;
	struct sigaction sa;
	sigset_t prevMask,blockMask;
	
	if(argc > 1 && strcmp(argv[1],"--help")==0){
		printf("Please enter the command line parameters\n");
		exit(EXIT_FAILURE);
	}
	
	printf("%s: PID is %ld\n",argv[0],(long)getpid());
	handlerSleepTime = (argc > 2) ? atoi(argv[2]) : 1;
	
	sa.sa_sigaction = realtime_sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigfillset(&sa.sa_mask);
	
	for(sig=1;sig < NSIG ; sig++)
		if(sig != SIGTSTP && sig != SIGQUIT)
			sigaction(sig,&sa,NULL);
			
	if(argc > 1){
		sigfillset(&blockMask);
		sigdelset(&blockMask,SIGINT);
		sigdelset(&blockMask,SIGTERM);
		
		if(sigprocmask(SIG_SETMASK,&blockMask,&prevMask) == -1){
			printf("New mask is not added successfully\n");
			exit(EXIT_FAILURE);
		}
		
		printf("%s: signals blocked - sleeping %s seconds\n",argv[0],argv[1]);
		sleep(atoi(argv[1]));
		printf("%s: sleep complete\n",argv[0]);
		
		if(sigprocmask(SIG_SETMASK,&prevMask,NULL) == -1){
			printf("Previous signal mask is not restored\n");
			exit(EXIT_FAILURE);
		}
	}
	
	while(!allDone)
		pause();		
}
