/*
Author		: Sahil Khan
Date		: 11th March 2017
Description	: This lab covers the concepts of message queues, one of the System V IPC 
		  mechanism in Linux OS. In this simple lab I have used the already created 
		  message queue and receive the data which was send at the other side.
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
	int msqid,opt,type = 0,flags = 0;
	ssize_t msgLen,maxBytes;
	struct mbuf msg;
	
	while((opt = getopt(argc, argv, "ent:x"))!= -1){
		switch(opt){
		case 'n':	flags |= IPC_NOWAIT;	break;
		case 'e':	flags |= MSG_NOERROR;	break;
		case 't':	type = atoi(optarg);	break;
		#ifdef MSG_EXCEPT
		case 'x':	flags |= IPC_EXCEPT;	break;
		#endif
		default:	pErr("Usage : %s [-n] msquid msg-type [msg-text]}",argv[0]);
		}
	}
	
	if(argc < optind +1 || argc > optind +2)
		pErr("Wrong number of arguments");
	
	msqid = atoi(argv[optind]);
	maxBytes = (argc > optind+1) ? atoi(argv[optind+1]) : MAX_MTEXT;
	
	if((msgLen = msgrcv(msqid,&msg,maxBytes,type,flags)) == -1)
		pErr("Message receive failed");

	pDbg("Received : type=%ld, length=%ld",msg.mtype,(long)msgLen);
	
	if(msgLen > 0)
		pDbg("body=%s",msg.mtext);

	exit(EXIT_SUCCESS);
}
