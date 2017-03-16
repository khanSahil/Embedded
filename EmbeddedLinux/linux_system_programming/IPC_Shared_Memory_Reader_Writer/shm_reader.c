/*
Author		: Sahil Khan
Date		: 16th March 2017
Description	: This lab covers the concepts of system V shared memory.In this program  
		  we are attching to the shared memory already created by the writer program.
		  After successfully attaching to the shared memory we read from the shared
		  memory segment and writes the data to the file provided at the command
		  line.
		  System V semaphores are used to synchronize the read and write on the shared
		  memory segment.
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

int
main(int argc, char *argv[])
{
    int semid, shmid, xfrs, bytes;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 0, 0);
    if (semid == -1)
        pErr("semget");

    shmid  = shmget(SHM_KEY, 0, 0);
    if (shmid == -1)
        pErr("shmget");

    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if (shmp == (void *) -1)
        pErr("shmat");

    for (xfrs = 0, bytes = 0; ; xfrs++) {
        if (reserveSem(semid, READ_SEM) == -1)          
            pErr("reserveSem");

        if (shmp->cnt == 0)                
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            pErr("partial/failed write");

        if (releaseSem(semid, WRITE_SEM) == -1)         
            pErr("releaseSem");
    }

    if (shmdt(shmp) == -1)
        pErr("shmdt");

    if (releaseSem(semid, WRITE_SEM) == -1)
        pErr("releaseSem");

    pDbg("Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
