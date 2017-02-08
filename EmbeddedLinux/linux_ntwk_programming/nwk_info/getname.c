/*
Author		: Sahil Khan
Date		: 5th Feb 2017
Description	: This program basically focusses on netwrok and host information that
		  you can get using host database functions. This program asks user to give 
		  host name as the command line parameters and uses that in gethostyname 
		  database fucntion to get all other details.
		  If the user doesn't provide the host name then this program calls gethostname
		  function to get the host name and uses that host name to acces further host 
		  details using gethostbyname function. It finally prints the associated inforamtion 
		  like aliases etc.
*/


#include "header.h"
int main(int argc, char**argv)
{
	char *host, **names, **addrs;
	struct hostent *hostinfo;
	
	if(argc == 1){
		char myname[256];
		gethostname(myname, 255);
		host = myname;
	}else
		host = argv[1];
		
	hostinfo = gethostbyname(host);
	
	if(!hostinfo){
		fprintf(stderr, "Cannot get info for host : %s\n",host);
		exit(1);
	}
	
	printf("results for host %s :\n",host);
	printf("Name : %s\n",hostinfo->h_name);	
	printf("Aliases : ");
	names = hostinfo->h_aliases;
	
	while(*names){
		printf(" %s",*names);
		names++;
	}
	printf("\n");
	
	if(hostinfo->h_addrtype != AF_INET){
		fprintf(stderr,"not an ip host..!!\n");
		exit(1);
	}
	
	addrs = hostinfo->h_addr_list;
	
	while(*addrs){
		printf(" %s", inet_ntoa(*(struct in_addr*)*addrs));
		addrs++;
	}
	printf("\n");
	exit(0);
}
