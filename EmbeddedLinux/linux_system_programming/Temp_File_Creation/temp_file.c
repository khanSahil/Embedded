/*
Author		: Sahil Khan
Date		: 9th Feb 2017
Description	: This lab introduces another important feature that linux system provides to user.
		  In this program we create a temporary file which gets deleted automatically when the 
		  program stops.
		  You can create file anywhere you want and provide the corresponding path of the directory 
		  along with the name of the file. Name of the file has to be in a specific format , it should
		  contain capital X as the last 6 characters in the file name as shown in the program.
		  The moment unlink is called the file gets unlinked but file gets deleted only after the
		  ececution of the program ends.
		  I am printing the name of the file,the last 6 characters denoted by X always changes
		  whenever you run the program, so you will see new file name each and every time you run 
		  the program.
		  Compilers usually makes temporary file during the compilation process.Similar to mkstemp()
		  we have another fucntion called as tmpfile() that serves the same purpose.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFF_SIZE 256


int main(int argc , char *argv[])
{
	int in_fd;
	char file_name[]="/tmp/tempXXXXXX";
	
	if((in_fd = mkstemp(file_name)) == -1){
		printf("Error : File was not generated");
		exit(EXIT_FAILURE);
	}
	
	printf("File created was : %s\n",file_name);
	unlink(file_name);
	
	if(close(in_fd) == 1){
		printf("Error : closing the generated file\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
	
	
