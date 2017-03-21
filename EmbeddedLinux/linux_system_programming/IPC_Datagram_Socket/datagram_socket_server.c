/*
Author		: Sahil Khan
Date		: 20th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a server which runs as a background process and receives
		  data from the client.
		  The moment it gets the data, it starts processing the data and converts each
		  character into Upper Case , sends the message back to the client.
*/

#include "header.h"

#define BACKLOG 5
int main(int argc, char *argv[])
{	
	struct sockaddr_un svaddr,claddr;
	int sfd,count=0;
	ssize_t numBytes;
	socklen_t len;
	char buf[BUF_SIZE];
	
	if((sfd = socket(AF_UNIX,SOCK_DGRAM,0)) == -1)
		pErr("socket() failed");
		
	if(remove(DGRAM_SOCKET_PATH) == -1 && errno != ENOENT)
		pErr("remove-%s",DGRAM_SOCKET_PATH);
		
	memset(&svaddr,0,sizeof(struct sockaddr_un));
	svaddr.sun_family = AF_UNIX;
	strncpy(svaddr.sun_path,DGRAM_SOCKET_PATH,sizeof(svaddr.sun_path) -1);
	
	if(bind(sfd,(struct sockaddr*)&svaddr,sizeof(struct sockaddr_un)) == -1)
		pErr("bind() failed");
		
	while(1){
		len = sizeof(struct sockaddr_un);
		if((numBytes = recvfrom(sfd,buf,BUF_SIZE,0,(struct sockaddr *)&claddr,&len)) == -1)
			pErr("recvfrom() failed");
					
		pDbg("Server received %ld bytes from %s",(long)numBytes,claddr.sun_path);
		
		while(count < numBytes){
			buf[count] = toupper((unsigned char)buf[count]);
			count++;
		}
		
		if(sendto(sfd,buf,numBytes,0,(struct sockaddr *)&claddr,len) != numBytes)
			pErr("sendto() failed");
	}	
}

