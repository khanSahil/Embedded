/*
Author		: Sahil Khan
Date		: 28th Jan 2017
Description	: This lab basically focusses on socket programming in Linux Environment.
			  This is client.c file which creates a socket and attaches itself to that
			  socket.After attaching, it writes to that socket and gets back a response 
			  from the server which is listening the same sockt.
			  Socket programming is one of the ways multiple process can communicate
			  with eac other and exchange status and data.This client server program
			  runs on the same local linux machine so newtwork access is involved in this 
			  program.
*/


#include "header.h"
int main()
{
	int sockfd,len,result;
	struct sockaddr_un address;
	char ch = 'A';
	
	sockfd = socket(AF_UNIX,SOCK_STREAM,0);

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	if(connect(sockfd,(struct sockaddr*)&address,len) == -1){
		perror("oops : client\n");
		exit(1);
	}
	
	write(sockfd,&ch,1);
	read(sockfd,&ch,1);
	printf("Character received from server : %c\n",ch);
	close(sockfd);
	exit(0);
}
