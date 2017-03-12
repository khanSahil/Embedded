/*
Author		: Sahil Khan
Date		: 11th March 2017
Description	: This lab covers the concepts of message queues, one of the System V IPC 
		  mechanism in Linux OS. In this simple lab I have discussed various ways of 
		  creating the message queue in LInux.Command line argumensts are processed 
		  using getopt() for file permissions and message queue creation way.
		  Debug Macros defined in header.h file will be used from now on-wards to print 
		  different debug messages be it error, warning or status.
*/

//debug Macros
//pErr(...)	
//pDdg(...)
//pWar(...)	

#include "header.h"

int main(int argc, char *argv[])
{	
	int numKeyFlags = 0,flags = 0,msqid,opt;
	unsigned int perm;
	long lkey;
	key_t key;
		
	while((opt = getopt(argc, argv, "cf:k:px"))!= -1){
		switch(opt){
		case 'c':
			flags |= IPC_CREAT;
			break;
		case 'f':
			if((key = ftok(optarg,1)) == -1)
				pErr("ftok() failed , key not generated");
			numKeyFlags++;
			break;
		case 'k':
			if(sscanf(optarg, "%li",&lkey) != 1)
				pErr("-k option require a numeric argument");
			key = lkey;
			numKeyFlags++;
			break;
		case 'p':
			key = IPC_PRIVATE;
			numKeyFlags++;
		case 'x':
			flags |= IPC_EXCL;
			break;
		default:
			pErr("Usage : %s [-cx] { -f pathname | -k key | -p [octal-perms] }",argv[0]);
		}
	}
	
	if(numKeyFlags != 1)
		pErr("Exactly one of the options -f,-k or -p must be supplied");
		
	perm = (optind == argc) ? (S_IRUSR|S_IWUSR):atoi(argv[optind]);	
	
	if((msqid = msgget(key,flags)) == -1)
		pErr("mssget() failed to create message queue");
		
	pDbg("MsgID : %d",msqid);	
	exit(EXIT_SUCCESS);
}
