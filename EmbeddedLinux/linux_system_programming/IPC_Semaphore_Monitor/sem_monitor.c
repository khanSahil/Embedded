/*
Author		: Sahil Khan
Date		: 14th March 2017
Description	: This lab covers the concepts of system V Semaphores. Ih this lab  
		  I printed out the list of information regarding the semaphores and 
		  which all semaphore are created and what all processess are waiting 
		  for the semaphores. When is the last time semaphore changed its value
		  or when is the last time semop() function was executed.
		  We just need to pass the identifier of already created semaphore as
		  the command line argument and it will display all the info of all the
		  semaphore associated with that sem id.
		  
*/

#include "header.h"

int main(int argc, char *argv[])
{	
	struct semid_ds ds;
	union semun arg,dummy;
	int semid,count;
	
	
	if(argc < 2 || strcmp(argv[1],"--help") == 0)
		pErr("Usage : %s semid",argv[0]);

	semid = atoi(argv[1]);
	arg.buf = &ds;
	if(semctl(semid,0,IPC_STAT,arg) == -1)
		pErr("semctl() failed\n");
	
	pDbg("Semaphore Changed	: %s",ctime(&ds.sem_ctime));
	pDbg("Last semop()		: %s",ctime(&ds.sem_otime));
	
	if((arg.array = calloc(ds.sem_nsems,sizeof(arg.array[0]))) == NULL)
		pErr("calloc() failed");
	
	if(semctl(semid,0,GETALL,arg) == -1)
		pErr("semctl() to get all sem failed");
		
	pDbg("Sem#	Value	SEMPID	SEMNCNT	SEMZCNT");
	for(count = 0 ; count < ds.sem_nsems; count++)
		pDbg("%3d	%5d	%5d	%5d	%5d",count,arg.array[count],
				semctl(semid,count,GETPID,dummy),
				semctl(semid,count,GETNCNT,dummy),
				semctl(semid,count,GETZCNT,dummy));
	exit(EXIT_SUCCESS);
}
