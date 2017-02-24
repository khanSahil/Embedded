/*
Author		: Sahil Khan
Date		: 23rd Feb 2017
Description	: This lab covers the concept related to zombie process in Linux OS. In this
          program a Zombie process is created and using ps command, the created zombie
          process was also shown on the terminal. System() system call is used to execute 
          the command. One more important feature is described i.e Zombie process cannot
          be killed by SIGKILL command so parent process have to call wait() system call
          eventually to remove the process from kernel process table.
          In this example also I tried to send a SIGKILL command to the newly created
          Zombie process and again did ps command just to show that Zombie process was
          not kill by SIGKILL command.
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<signal.h>
#include<libgen.h>

#define CMD_SIZE 200

int main(int argc, char *argv[])
{
	char command[CMD_SIZE];
	pid_t pid_value;
	
	setbuf(stdout,NULL);
	
	printf("Parent PID : %ld\n",(long)getpid());
	switch(pid_value = fork())
	{
		case -1:
			printf("Child process not created successfully\n");
			exit(EXIT_SUCCESS);
			break;
		case 0:
			printf("Child PID : %ld exiting\n",(long)getpid());
			_exit(EXIT_SUCCESS);
			break;
		default :
			printf("Parent Sleeping\n");
			sleep(3);
			snprintf(command,CMD_SIZE,"ps | grep %s",basename(argv[0]));
			command[CMD_SIZE - 1] = '\0';
			system(command);
						
			if(kill(pid_value,SIGKILL) == -1){
				printf("Error : Kill Command not sent\n");\
				exit(EXIT_SUCCESS);
			}
			sleep(3);
			printf("After sending SIGKILL to Zombie(PID = %ld):\n",(long)pid_value);
			system(command);
			exit(EXIT_SUCCESS);
		}
}
