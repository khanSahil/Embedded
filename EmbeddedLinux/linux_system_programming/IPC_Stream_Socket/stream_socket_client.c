/*
Author		: Sahil Khan
Date		: 19th March 2017
Description	: This lab covers the concepts of system V sockets programming. In this 
		  example I have created a client and connected it to a server already created
		  before. This client reads the file (say file_1) from the standard input and writes it the
		  write end of the socket.
		  The server on the other side reads the data from the socket and writes it to another file
		  (say File_2). After the operation is completed , we can do "diff File_1 File_2", to see 
		  both files contains the same data.
*/

#include "header.h"

int main(int argc, char *argv[])
{	


	struct sockaddr_un addr;
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];
	
	if((sfd = socket(AF_UNIX,SOCK_STREAM,0)) == -1)
		pErr("socket() failed");
		
	memset(&addr,0,sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path,STREAM_SOCKET_PATH,sizeof(addr.sun_path) -1);
	
	if(connect(sfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_un)) == -1)
		pErr("connect() failed");
		
	while((numRead = read(STDIN_FILENO,buf,BUF_SIZE)) > 0){
		if(write(sfd,buf,numRead) != numRead)
			pErr("write() faield on STDOUT_FILENO");
	}
	
	if(numRead == -1)
		pErr("read() failed");
	exit(EXIT_SUCCESS);
}
