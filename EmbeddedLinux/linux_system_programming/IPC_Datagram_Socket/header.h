#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>

#define DGRAM_SOCKET_PATH "/tmp/dgram_sock"
#define BUF_SIZE 10

#define DEBUG 1
#ifdef DEBUG

#define pDbg(...)	{printf("Trace_Debug: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);}
#define pWar(...)	{printf("Trace_warning: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);}
#define pErr(...)	{printf("Trace_Error: "); printf(__VA_ARGS__);printf(" - (%s:%s:%d)\n",__FILE__,__func__,__LINE__);\
					 exit(EXIT_FAILURE);}
#else
#define pErr(...)	
#define pDbg(...)	
#define pWar(...)

#endif



