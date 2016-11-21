#!/bin/bash

# This script basically log into BBB and turn ON or OFF
# all the on-board LEDs. I like to turn them OFF whenever
# I use BBB as flashing of the LEDs distracts me. You can 
# can either turn ON or OFF the LEDs based on the argument
# that you pass. 1 = LED_ON and 0 = LED_OFF.


if [ $# -ne 1 ]
then
	echo "Please enter the status for LEDs : 1=LED_ON, 2=LED_OFF"
	exit 0
fi

while true;
do
  ping -c1 192.168.7.2
  if [ $? -eq 0 ]
  then 
    echo "Beagle Bone Black Found"
	ssh root@192.168.7.2 "./led_off.sh $1"
    exit 0
  fi
done
