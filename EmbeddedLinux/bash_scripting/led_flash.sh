#!/bin/bash

# This script is used to toggle the externally connected LED at GPIO49.
# Gpio number is passed as an argument to this script and then the required
# gpio directory is created inside the path /sys/class/gpio/.After toggling
# the LED, the gpio directory is deleted from sys/class/gpio/ path.


#declaring path to the GPIO directory
GPIO_PATH=/sys/class/gpio

#This loop basically check if the reguired 
#gpio directory is present or not and 
#correspondingly sets dir_[resent variable

dir_list=`ls $GPIO_PATH`
dir_present=0
for dir in $dir_list
do
	if [ "$dir" == "gpio$1" ]
	then
		echo "Directort gpio$1 present already"
		dir_present=1	
		break;
	else
		continue;
	fi
done


# This checks and creates required gpio directory if necessary
if [ $dir_present -ne 1 ]
then
	echo "Directory not present : Creating gpio$1"
	echo $1 > $GPIO_PATH/export
fi

# defining the direction of the GPIO
LED_PATH=$GPIO_PATH/gpio$1
echo $LED_PATH
echo "out" > $LED_PATH/direction

counter=0
# simple loop to toggle the GPIO
while [ $counter -lt 10 ]
do
	echo 1 > $LED_PATH/value
	sleep 1
	echo 0 > $LED_PATH/value
	sleep 1
	counter=$[ $counter + 1 ]
done

#deleting the gpio directory created above
echo $1 > $GPIO_PATH/export
