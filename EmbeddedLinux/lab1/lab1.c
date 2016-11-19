/*
Author		: Sahil Khan
Date		: 18th Nov 2016
Description	: This lab basically focusses on accessing the on-board LEDs of BeagleBoneBlack.
			 It simply opens the path and write to it to trigger LED0 or turn off LED0.It
			 checks what parameter you are passing and accordinly turns ON or OFF LED0.This
			 BBB runs kernel verison 3.8.13-bone81. 
*/

// Including the required Header-Files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// defining the path 
#define LED0_Blinking_On "/sys/class/leds/beaglebone:green:usr0/trigger"
#define LED0_Blinking_Off "/sys/class/leds/beaglebone:green:usr0/brightness"
#define num_elements 1

int main(int argc, char *argv[])
{
	FILE *fp=NULL;
	int size=0;
	
	if(strcmp(argv[1],"timer")==0){

		fp=fopen(LED0_Blinking_On,"w");

		if(fp == NULL){
			printf("ERROR : Trigger : The LED0 path cannot be open\n");
			exit(0);
		}

		size=fwrite(argv[1],strlen(argv[1]),num_elements,fp);

		if(size != num_elements){
			printf("ERROR : Trigger : Write to LED0 failed\n");
			exit(0);
		}
	}
	else
	{
		fp=fopen(LED0_Blinking_Off,"w");
		if(fp == NULL){
			printf("ERROR : Brightness : The LED0 path cannot be open\n");
			exit(0);
		}   

		size=fwrite(argv[1],strlen(argv[1]),num_elements,fp);
		if(size != num_elements){
			printf("ERROR : Brightness : Write to LED0 failed\n");
			exit(0);
		}
	}

	printf("Successfully wrote to On-Board LED0\n");
	fclose(fp);
	return 0;
}
