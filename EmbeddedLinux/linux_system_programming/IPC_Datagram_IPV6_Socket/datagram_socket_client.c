/*
Author		: Sahil Khan
Date		: 21th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a client that send data to and receive response from
		  server already before.Command line arguments were parsed and send to server 
		  for further proessing.
		  The server on the other side reads the data from the socket and sends back to
		  client after converting each character of data into Upper Case.
		  This client server program runs on the same host but uses the network to exchange
		  data between client and server.
*/

#include "header.h"

int main(int argc, char *argv[])
{	
	struct sockaddr_in6 svaddr,claddr;
	int sfd,count;
	size_t msgLen;
	ssize_t numBytes;
	char resp[BUF_SIZE];
	
	if(argc < 3 || strcmp(argv[1],"--help") == 0)
		pErr("Usage : %s host-address msg...",argv[0]);
		
	if((sfd = socket(AF_INET6,SOCK_DGRAM,0)) == -1)
		pErr("socket() failed");
	
	memset(&svaddr,0,sizeof(struct sockaddr_in6));
	svaddr.sin6_family = AF_INET6;
	svaddr.sin6_port = htons(PORT_NUM);
	
	if(inet_pton(AF_INET6,argv[1],&svaddr.sin6_addr) <= 0)
		pErr("inet_pton() failed for address '%s'",argv[1]); 
		
	for(count = 2; count < argc; count++){
		msgLen = strlen(argv[count]);
		
		if (sendto(sfd, argv[count], msgLen, 0, (struct sockaddr *) &svaddr,
                    sizeof(struct sockaddr_in6)) != msgLen)
            pErr("sendto");
            
		if(recvfrom(sfd,resp,BUF_SIZE,0,NULL,NULL) == -1)
			pErr("recvfrom() failed");
			
		pDbg("Response %d : %.*s",count-1,(int)numBytes,resp);
	}
	exit(EXIT_SUCCESS);
}

