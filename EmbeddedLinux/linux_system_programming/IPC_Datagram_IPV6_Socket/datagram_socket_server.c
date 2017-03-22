/*
Author		: Sahil Khan
Date		: 21th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a server which runs as a background process and receives
		  data from the client.
		  The moment it gets the data, it starts processing the data and converts each
		  character into Upper Case , sends the message back to the client.
		  This client server program runs on the same host but uses the network to exchange
		  data between client and server.
*/

#include "header.h"

#define BACKLOG 5
int main(int argc, char *argv[])
{	
	struct sockaddr_in6 svaddr,claddr;
	int sfd,count=0;
	ssize_t numBytes;
	socklen_t len;
	char buf[BUF_SIZE];
	char claddrStr[INET6_ADDRSTRLEN];
	
	if((sfd = socket(AF_INET6,SOCK_DGRAM,0)) == -1)
		pErr("socket() failed");
		
	memset(&svaddr,0,sizeof(struct sockaddr_in6));
	svaddr.sin6_family = AF_INET6;
	svaddr.sin6_addr = in6addr_any;
	svaddr.sin6_port = htons(PORT_NUM);
	
	if(bind(sfd,(struct sockaddr*)&svaddr,sizeof(struct sockaddr_in6)) == -1)
		pErr("bind() failed");
		
	pDbg("Server Started : listening....");
	while(1){
		len = sizeof(struct sockaddr_in6);
		if((numBytes = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&claddr,&len)) == -1)
			pErr("recvfrom() failed");

		if(inet_ntop(AF_INET6,&claddr.sin6_addr,claddrStr,INET6_ADDRSTRLEN) == NULL){
			pErr("Could not convert client address to string");
		}	
		else
			pDbg("Server received %ld bytes from (%s,%u)",(long)numBytes,claddrStr,ntohs(claddr.sin6_port));
		
		while(count < numBytes){
			buf[count] = toupper((unsigned char)buf[count]);
			count++;
		}
		
		if(sendto(sfd,buf,numBytes,0,(struct sockaddr *)&claddr,len) != numBytes)
			pErr("sendto() failed");
	}	
}

