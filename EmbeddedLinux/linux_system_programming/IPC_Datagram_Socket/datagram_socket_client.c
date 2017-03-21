/*
Author		: Sahil Khan
Date		: 20th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a client that send data to and receive response from
		  server already before.Command line arguments were parsed and send to server 
		  for further proessing.
		  The server on the other side reads the data from the socket and sends back to
		  client after converting each character of data into Upper Case.
*/

#include "header.h"

int main(int argc, char *argv[])
{	
	struct sockaddr_un svaddr,claddr;
	int sfd,count=0;
	size_t msgLen;
	ssize_t numBytes;
	char resp[BUF_SIZE];
	
	if(argc < 2 || strcmp(argv[1],"--help") == 0)
		pErr("Usage : %s msg...",argv[0]);
		
	if((sfd = socket(AF_UNIX,SOCK_DGRAM,0)) == -1)
		pErr("socket() failed");
		
	memset(&claddr,0,sizeof(struct sockaddr_un));
	claddr.sun_family = AF_UNIX;
	snprintf(claddr.sun_path,sizeof(claddr.sun_path),"/tmp/dgram_sock.%ld",(long)getpid());
	
	if(bind(sfd,(struct sockaddr *)&claddr,sizeof(struct sockaddr_un)) == -1)
		pErr("bind() failed");
	
	memset(&svaddr,0,sizeof(struct sockaddr_un));
	svaddr.sun_family = AF_UNIX;
	strncpy(svaddr.sun_path,DGRAM_SOCKET_PATH,sizeof(svaddr.sun_path) -1);
		
	for(count = 1; count < argc; count++){
		msgLen = strlen(argv[count]);
		if(sendto(sfd,argv[count],msgLen,0,(struct sockaddr *)&svaddr,sizeof(struct sockaddr_un)) != msgLen)
			pErr("sendto() failed");
		
		if(recvfrom(sfd,resp,BUF_SIZE,0,NULL,NULL) == -1)
			pErr("recvfrom() failed");
			
		pDbg("Response %d : %.*s",count,(int)numBytes,resp);
	}

	remove(claddr.sun_path);
	exit(EXIT_SUCCESS);
}
