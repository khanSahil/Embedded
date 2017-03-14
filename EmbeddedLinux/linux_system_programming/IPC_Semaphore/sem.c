/*
Author		: Sahil Khan
Date		: 13th March 2017
Description	: This lab covers the concepts of system V Semaphores. Ih this lab  
		  semaphore id gets generated and then some operations are performed using
		  the already created semaphore identifier. 
		  Depending upon the command line arguments , either the sempahore is created
		  or semaphore operations are performed on the already creatd sem id.
*/

#include "header.h"

int main(int argc, char *argv[])
{	
	int semid;
	
	if(argc < 2 || argc > 3 || strcmp(argv[1],"--help") == 0)
		pErr("Usage : %s init-value or : %s semid operation",argv[0],argv[0]);

	if(argc == 2){
		union semun arg;
		
		if((semid = semget(IPC_PRIVATE,1,S_IRUSR|S_IWUSR)) == -1)
			pErr("semget() failed to creatd sem identifier");
			
		arg.val = atoi(argv[1]);
		if(semctl(semid,0, SETVAL,arg) == -1)
			pErr("semctl() failed to set the sem value");
			
		pDbg("Semaphore ID = %d",semid);
	} else {
		struct sembuf sop;
		
		semid = atoi(argv[1]);
		sop.sem_num = 0;
		sop.sem_op = atoi(argv[1]);
		sop.sem_flg = 0;
		
		pDbg("%ld: about to semop",(long)getpid());
		if(semop(semid,&sop,1) == -1)
			pErr("semop() failed");
		pDbg("%ld: semop completed",(long)getpid());
	}
	exit(EXIT_SUCCESS);
}
