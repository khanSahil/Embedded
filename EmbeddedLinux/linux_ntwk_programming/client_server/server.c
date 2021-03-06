/*
Author		: Sahil Khan
Date		: 28th Jan 2017
Description	: This lab basically focusses on socket programming in Linux Environment.
			  This is server.c file which creates a socket and attaches itself to that
			  socket.After attaching, it writes to that socket, it waits to listen if
			  any client is trying to communicate through that socket. This server handles
			  one client at a time as multiple client handling is not supported in this 
			  program.
			  Socket programming is one of the ways multiple process can communicate
			  with each other and exchange status and data.This client server program
			  runs on the same local linux machine so no newtwork access is involved in 
			  this program.
*/

#include "header.h"
int main()
{
	int server_sockfd,client_sockfd;
	socklen_t server_len,client_len;
	struct sockaddr_un server_address,client_address;
	
	unlink("server_socket");
	server_sockfd = socket(AF_UNIX,SOCK_STREAM,0);
	
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = (socklen_t)sizeof(server_address);
	bind(server_sockfd, (struct sockaddr*)&server_address,server_len);
	
	listen(server_sockfd, 5);
	
	while(1){
		char ch;
		printf("Server Waiting\n");
		
		client_len = (socklen_t)sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_address,&client_len);
		read(client_sockfd,&ch,1);
		ch++;
		write(client_sockfd,&ch,1);
		close(client_sockfd);
	}
	exit(0);
}
