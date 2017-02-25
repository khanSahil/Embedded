/*
Author		: Sahil Khan
Date		: 24th Feb 2017
Description	: This lab covers the concept related to standard macros defined in header file
                  <sys/wait.h> to dissect the wait status value. When applied to a status value
                  returned by wait() or waitpid(), only one of the macros in the list will return
                  true. In this program 4 macros are used WIFEXITED(status),WIFSIGNALED(status),
                  WIFSTOPPED(status) and WIFCONTINUED(status). 
                  When you run the process at the background and send the signals then the corre-
                  -sponding signal status will be printed inside printWaitStatus() function.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>

void printWaitStatus(const char *msg,unsigned int status)
{
	if(msg != NULL)
		printf("%s\n",msg);
	
	if(WIFEXITED(status))
		printf("Child Exited : status %d\n",WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		printf("Child Killed by signal : status %d\n",WTERMSIG(status));
	else if(WIFCONTINUED(status))
		printf("Child Continued\n");
	else if(WIFSTOPPED(status))
		printf("Child was stopped : status %d\n",WSTOPSIG(status));
	else
		printf("Invalid Status : status %d\n",status);	
}

int main(int argc, char *argv[])
{
	int status;
	pid_t pid_value;
	
	if(argc > 1 && strcmp(argv[1],"--help") == 0)
		printf("Usage : ./process <status>\n");

	switch(fork())
	{
		case -1:
			printf("Child process not created successfully\n");
			exit(EXIT_SUCCESS);
		case 0:
			printf("Child started with PID : %ld\n",(long)getpid());
			if(argc > 1)
				exit(atoi(argv[1]));
			else
				for(;;)
					pause();
			exit(EXIT_FAILURE);
		default :
			for(;;){
				pid_value = waitpid(-1,&status,WUNTRACED | WCONTINUED);
				if(pid_value == -1){
					printf("Error : wait() failed \n");
					exit(EXIT_FAILURE);
				}
				
				printf("waitpid() returned: PID=%ld; status = 0x%04x (%d,%d)\n",(long)pid_value,
					  (unsigned int)status, status >> 8 , status & 0xff);\
				printWaitStatus(NULL,status);
				if(WIFEXITED(status) || WIFSIGNALED(status))
					exit(EXIT_SUCCESS);
			}
	}
}
