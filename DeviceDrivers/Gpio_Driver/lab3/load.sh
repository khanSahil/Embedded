#!/bin/bash

case "$1" in

	1) 
		if [ `make` && `insmod $2.ko` ] 
		then
			echo "$2.ko loaded successfully : Opening the Kernel Logs"
			sleep 3
			tail -f -n 200 /var/log/kern.log
		else
			echo "Failed to load the module"
		fi;;
			
	0)
		echo "Unloading the $2.ko Module from the System"
		sudo rmmod $2 ;;
	*) 
		echo "Please type following commands" ; 
		echo "./load.sh 1 <.ko file name>- For loading the Module"
		echo "./load.sh 0 <.ko file name>- For Unloading the Module";;
esac

