/*
Author		: Sahil Khan
Date		: 19th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a server which runs as a background process and listens
		  to any incoming connection from the clients.
		  The moment it accepts a connection, it starts reading from that socket , where 
		  the client is writing after reading from file.(say File_1),and writes it to a 
		  file (say File_2).After the operation is completed , we can do "diff File_1 File_2", 
		  to see both files contains the same data.
*/

#include "header.h"

#define BACKLOG 5
int main(int argc, char *argv[])
{	
	struct sockaddr_un addr;
	int sfd,cfd;
	ssize_t numRead;
	char buf[BUF_SIZE];
	
	if((sfd = socket(AF_UNIX,SOCK_STREAM,0)) == -1)
		pErr("socket() failed");
		
	if(remove(STREAM_SOCKET_PATH) == -1 && errno != ENOENT)
		pErr("remove-%s",STREAM_SOCKET_PATH);
		
	memset(&addr,0,sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,STREAM_SOCKET_PATH,sizeof(addr.sun_path) -1);
	
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_un)) == -1)
		pErr("bind() failed");
	
	if(listen(sfd,BACKLOG) == -1)
		pErr("listen() failed");
		
	pDbg("Waiting for any client incomming connection");
	while(1){
		if((cfd = accept(sfd,NULL,NULL)) == -1)
			pErr("accept() failed");
			
		pDbg("One incomming connection accepted");
		
		while((numRead = read(cfd,buf,BUF_SIZE)) > 0){
			if(write(STDOUT_FILENO,buf,numRead) != numRead)
				pErr("write() faield on STDOUT_FILENO");
		}
		
		if(numRead == -1)
			pErr("read() failed");
		
		if(close(cfd) == -1)
			pErr("close() on client failed");
	}	
}

