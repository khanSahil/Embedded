#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/shm.h>

#ifndef BINARY_SEMS_H       
	#define BINARY_SEMS_H
#endif

#define TRUE	1
#define FALSE	0

#define SHM_KEY 0x1234
#define SEM_KEY 0x4567

#define WRITE_SEM	1
#define READ_SEM	0

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

extern bool bsUseSemUndo;            
extern bool bsRetryOnEintr;          

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
                                           
int initSemAvailable(int semId, int semNum);
int initSemInUse(int semId, int semNum);
int reserveSem(int semId, int semNum);
int releaseSem(int semId, int semNum);

struct shmseg{
	int cnt;
	char buf[BUF_SIZE];
};

union semun {
   int              val;    
   struct semid_ds *buf;    
   unsigned short  *array; 
   struct seminfo  *__buf;  
                             
};


#define DEBUG 1
#define pErr(...)	{printf("Trace_Error: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);exit(EXIT_FAILURE);}
#define pDbg(...)	if (DEBUG) {printf("Trace_Debug: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);}
#define pWar(...)	if (DEBUG) {printf("Trace_warning: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);}



