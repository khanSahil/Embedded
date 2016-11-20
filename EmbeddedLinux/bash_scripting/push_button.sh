#!/bin/bash

# This script is used to read the PUSH button status continuously connected 
# at GPIO115.Required Gpio number is passed as an argument to this script and
# the directory is created inside /sys/class/gpio path.That GPIO pin is made
# as an input pin to read the Push Button status.To terminate from a while loop
# handler is defined to recieve ctrl+c trap, which deletes the directory created 
# the gpio and exit. 

#declaring path to the GPIO directory
GPIO_PATH=/sys/class/gpio

#This loop basically check if the reguired 
#gpio directory is present or not and 
#correspondingly sets dir_present variable

#saving the gpio number passed to be used in signal handler
gpio_num=$1

#defining the signal hander for ctrl+c trap
trap ctrl_c INT
function ctrl_c(){
        echo "** Trapped CTRL-C **"
        echo "Deleting the GPIO$gpio_num directory for /sys/class/gpio path"
        echo $gpio_num > $GPIO_PATH/unexport
        exit 0
}

# checking if the gpio number is passed as an argument or not
if [ "$#" -ne 1 ]; then
    echo "ERROR : Illegal number of parameters"
	echo "Please pass the GPIO number as an argumet"
	exit 0
fi

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
echo "in" > $LED_PATH/direction

counter=0
# simple loop to toggle the GPIO
while :
do
	echo -n "Push Button Reads : "
	cat  $LED_PATH/value
	sleep 1
done

