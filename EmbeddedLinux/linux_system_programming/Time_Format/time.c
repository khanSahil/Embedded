/*
Author		: Sahil Khan
Date		: 9th Feb 2017
Description	: This lab introduces some of the important aspects of time formatting in linux system.
		  In this program various formats in which time can be displayed on linux machine is
		  discussed. You can print the time in seconds format , or a proper string format depending
		  upon the kind of function you are using. You can also even convert one format into another 
		  format using some of the functions.
		  Linux system represents time internally in a variable of type time_t as a measure of seconds
		  since Epoch; that is, since midnight on the morning of 1 January 1970 UTC.
*/


#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

int main()
{
	time_t t;
	struct tm *gmp,*locp,gm,loc;
	struct timeval tv;
	
	t = time(NULL);
	printf("\n=============================================================================\n");
	printf("Seconds since Epoch (1 Jan 1970):		%ld",(long)t);
	printf("(about %6.3f years)\n",t/SECONDS_IN_TROPICAL_YEAR);
	
	if(gettimeofday(&tv,NULL) == -1){
		printf("Error in getting time of the day\n");
		exit(EXIT_FAILURE);
	}
	
	printf("gettimeofday() function returned		%ld secs,%ld microsecs\n",(long)tv.tv_sec,tv.tv_usec);
	
	if((locp = localtime(&t)) == NULL){
		printf("Error in getting local time\n");
		exit(EXIT_FAILURE);
	}
	
	loc = *locp;
	
	printf("\nBroken down by localtime() :\n");
	printf(" year=%d mon=%d mday=%d hour=%d min=%d sec=%d",loc.tm_year,loc.tm_mon,loc.tm_mday,loc.tm_hour,loc.tm_min,loc.tm_sec);
	printf(" wday=%d yday=%d isdst=%d\n\n",loc.tm_wday,loc.tm_yday,loc.tm_isdst);
	
	printf("ctime() formats the time() value as :		%s",ctime(&t));
	
	printf("mktime() for localtime() value:			%ld sec\n",(long)mktime(&loc));
	printf("\n=============================================================================\n");
	exit(EXIT_SUCCESS);
}
