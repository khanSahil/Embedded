/*
Author		: Sahil Khan
Date		: 11th March 2017
Description	: This lab covers the concepts of message queues, one of the System V IPC 
		  mechanism in Linux OS. In this simple lab I have used the already created 
		  message queue and send the data which will be received on the other side.
		  This process uses the identifier created by msg_queue_create.c process.
*/

#include "header.h"

#define MAX_MTEXT 1024

struct mbuf{
	long mtype;
	char mtext[MAX_MTEXT];
};

int main(int argc, char *argv[])
{	
	int msgLen = 0,flags = 0,msqid,opt;
	struct mbuf msg;
	
	while((opt = getopt(argc, argv, "n"))!= -1){
		switch(opt){
		case 'n':
			flags |= IPC_NOWAIT;
			break;
		default:
			pErr("Usage : %s [-n] msquid msg-type [msg-text]}",argv[0]);
		}
	}
	
	msqid = atoi(argv[optind]);
	msg.mtype = atoi(argv[optind+1]);
	if(argc > optind+2){
		msgLen = strlen(argv[optind + 2]) +1;
		if(msgLen > MAX_MTEXT)
			pErr("Length of the command line message is greater than the max mtext size");
		memcpy(msg.mtext,argv[optind+2],msgLen);
	}else
		msgLen = 0;
		
	if(msgsnd(msqid,&msg,msgLen,flags) == -1)
		pErr("msgsnd() failed to send the data");
	
	exit(EXIT_SUCCESS);
}
