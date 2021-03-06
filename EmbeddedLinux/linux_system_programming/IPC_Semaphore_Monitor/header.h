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
#include <time.h>

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};


#define DEBUG 1
#define pErr(...)	{printf("Trace_Error: "); printf(__VA_ARGS__);printf(" - %s:%d\n",__FILE__,__LINE__);exit(EXIT_FAILURE);}
#define pDbg(...)	if (DEBUG) {printf("Trace_Debug: "); printf(__VA_ARGS__);printf(" - %s:%d\n",__FILE__,__LINE__);}
#define pWar(...)	if (DEBUG) {printf("Trace_warning: "); printf(__VA_ARGS__);printf(" - %s:%d\n",__FILE__,__LINE__);}
