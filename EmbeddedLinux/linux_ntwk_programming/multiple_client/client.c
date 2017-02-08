/*
Author		: Sahil Khan
Date		: 29th Jan 2017
Description	: This lab basically focusses on socket programming in Linux Environment.
		  This is client.c file which creates a socket and attaches itself to that
		  socket.After attaching, it writes to that socket and gets back a response 
		  from the server which is listening the same socket. This client program 
		  talks to server using internal loopback network.The server is designed in
		  such a way that it accepts connection from any address.
		  Socket programming is one of the ways multiple process can communicate
		  with each other and exchange status and data.This client server program
		  runs on the same local linux machine using the internal loop back ip 
		  address 127.0.0.1.
*/

#include "header.h"

int main()
{
	int sockfd, len, result;
	struct sockaddr_in address;
	char ch = 'A';
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);
	len = sizeof(address);
	
	if(connect(sockfd,(struct sockaddr*)&address,len) == -1){
		perror("oops : network client\n");
		exit(1);
	}
	
	write(sockfd,&ch,1);
	read(sockfd,&ch,1);
	printf("Character received from server : %c\n",ch);
	close(sockfd);
	exit(0);
}
