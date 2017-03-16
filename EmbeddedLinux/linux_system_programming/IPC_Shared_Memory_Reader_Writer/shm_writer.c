/*
Author		: Sahil Khan
Date		: 16th March 2017
Description	: This lab covers the concepts of system V shared memory. In this program  
		  we created a shared memory segment, in which we are writing the data,after 
		  reading it from the file , provided at command line.
		  This shared memory is available to reader also which reads it and writes it
		  to the file provided at the command line.
		  System V semaphores are used to synchronize the read and write between reader
		  and writer so that the data is not lost.
*/

#include "header.h"
bool bsUseSemUndo = FALSE;
bool bsRetryOnEintr = TRUE;

 int               
 initSemAvailable(int semId, int semNum){
     union semun arg;
     arg.val = 1;
     return semctl(semId, semNum, SETVAL, arg);
 }
 
 int                     
 initSemInUse(int semId, int semNum){
     union semun arg;
     arg.val = 0;
     return semctl(semId, semNum, SETVAL, arg);
 }
 
 int                  
 reserveSem(int semId, int semNum){
     struct sembuf sops;
     sops.sem_num = semNum;
     sops.sem_op = -1;
     sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;
 
     while (semop(semId, &sops, 1) == -1)
         if (errno != EINTR || !bsRetryOnEintr)
             return -1;
     return 0;
 }
 
 int                  
 releaseSem(int semId, int semNum)
 {
     struct sembuf sops;
     sops.sem_num = semNum;
     sops.sem_op = 1;
     sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;
     return semop(semId, &sops, 1);
 }

int main(int argc, char *argv[])
{	
     int semid, shmid, bytes, xfrs;
     struct shmseg *shmp;
     union semun dummy;
     
     semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
     if (semid == -1)
         pErr("semget");
 
     if (initSemAvailable(semid, WRITE_SEM) == -1)
         pErr("initSemAvailable");
     if (initSemInUse(semid, READ_SEM) == -1)
         pErr("initSemInUse");
 
     shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
     if (shmid == -1)
         pErr("shmget");
 
     shmp = shmat(shmid, NULL, 0);
     if (shmp == (void *) -1)
         pErr("shmat");
 
     for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) {
         if (reserveSem(semid, WRITE_SEM) == -1)        
             pErr("reserveSem");
 
         shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
         if (shmp->cnt == -1)
             pErr("read");
 
         if (releaseSem(semid, READ_SEM) == -1)          
             pErr("releaseSem");
 
         if (shmp->cnt == 0)
             break;
     }
 
     if (reserveSem(semid, WRITE_SEM) == -1)
         pErr("reserveSem");
 
     if (semctl(semid, 0, IPC_RMID, dummy) == -1)
         pErr("semctl");
     if (shmdt(shmp) == -1)
         pErr("shmdt");
     if (shmctl(shmid, IPC_RMID, 0) == -1)
         pErr("shmctl");
 
     pDbg("Sent %d bytes (%d xfrs)\n", bytes, xfrs);
     exit(EXIT_SUCCESS);
}

