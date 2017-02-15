/*
Author		: Sahil Khan
Date		: 13th Feb 2017
Description	: This lab covers another important aspect related to signals in
		  Linux System, it explores strsignal() function which is very powerful
		  fuction in mapping the signal number to their corresponding signal
		  description. It is very helpful in debugging purpose or you can used it
		  to print on the debug console about signal that the current process has
		  has handeled in more human readable format.
		  This program takes the signal number as the command line parameter and 
		  prints the corresponding description of the signal on standard output.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *sig_name;
	int sig_num;
	if(argc !=2 || strcmp(argv[1],"--help")==0){
		printf("Usage : ./signal <signal_num>\n");
		exit(EXIT_SUCCESS);
	}
	sig_num = atoi(argv[1]);
	sig_name = strsignal(sig_num);
	printf("Signal : %s\n",sig_name);
	exit(EXIT_SUCCESS);
}
