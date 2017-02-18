#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
int globalData = 111;


int main()
{
	pid_t pid_value;
	int localData = 222;
	switch(pid_value = fork()){
	case -1:
		printf("Child process not created successfully\n");
		exit(EXIT_FAILURE);
	case 0:
		globalData *=2;
		localData *=2;
		break;
	default:
		sleep(3);
		break;
	}
	
	printf("PID = %ld (%s) globalData = %d localData = %d\n",
					(long)getpid(),pid_value == 0 ? "child" : "parent",
					globalData,localData);
					
	exit(EXIT_SUCCESS);
}
