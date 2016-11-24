#!/bin/bash
#  Author : Sahil Khan

#  Description:
#  		This script searches for any device attached to i2c bus
#  		in the /dev directory. For all the i2c buses available, 
#  		its goint to scan each bus and if it finds any device it
#  		will display the address as well as the date of that device.

i2c_buses=`ls -l /dev/ | grep i2c | awk '{print $6}'`
echo  "Available i2c-busses : " $i2c_buses
echo "================================================="
for bus in $i2c_buses
do
echo "===============Searching i2c-$bus bus==============="
	start_scanning=0
	found=0
	data=`i2cdetect -y $bus`
	
	for i in $data
	do
		if [ "$i" == "00:" ];then start_scanning=1;fi
		if [ $start_scanning -eq 1 ];then	
			value=`echo $i | tr --delete :`
			if [ "$i" != "--" ] && [ "$i" != "$value:" ];then
				echo "Device is at address 0x$i"
				found=1
			fi
		fi
	done

	if [ $found -ne 1 ]; then echo "No Device found";fi
echo "================================================="
done


