/*
Author		: Sahil khan
Date		: 12/04/2016
Description	: This program is used to access the device on i2c-2 inside my laptop.It is 
		  an eeprom having an adderess 0x50.It explores basic read and write 
		  related to I2C protocol where I read 128 bytes and stores it in buff.
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h> 
#include <unistd.h>
#include <stdint.h>

#define DEVICE_ADDRESS 0x50
#define BUFF_SIZE 128

int main(){
	int file,i,read_bytes=0;
	uint8_t buff[BUFF_SIZE];
	printf("\nStarting I2C Application\n");

	if((file=open("/dev/i2c-2", O_RDWR)) < 0){
		perror("failed to open the bus\n");
		return 1;
	}
	
	if(ioctl(file, I2C_SLAVE, DEVICE_ADDRESS) < 0){
		perror("Failed to connect to the sensor\n");
		return 1;
	}

	if(i2c_smbus_write_byte(file,0x0) < 0){
		printf("Failed to Write to Device %d\n",DEVICE_ADDRESS);
		exit(1);
	}

	if(read(file,buff,BUFF_SIZE ) != BUFF_SIZE){
		printf("Read was not successfull\n");
		exit(1);
	}

	for(i=0;i<128;i++)
	{	
		if(i%16 == 0)
			printf("\n");
		printf("0x%x	",buff[i]);
	}
	printf("\n");   
	close(file);
	return 0;
}

