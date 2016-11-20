#!/bin/bash

case "$1" in
 
 	1) 
 		sudo make ;
 		if [ $? -eq 0 ]
 		then 
 			sudo insmod $2.ko; 
 			if [ $? -eq 0 ]
 			then
 				echo "$2.ko loaded successfully"
 				echo "Opening the Kernel Logs in 3 Seconds"
 				sleep 3
 				tail -f -n 200 /var/log/kern.log
 			else
 				echo "Module not loaded successfully"
 			fi
 		else
 			echo "Module not generated"
 		fi;;
 			
 	0)
 		echo "Unloading the $2.ko Module from the System"
 		sudo rmmod $2 
		tail -f -n 10 /var/log/kern.log;;
 	*) 
 		echo "Please type following commands" ; 
 		echo "./load.sh 1 <.ko file name>- For loading the Module"
 		echo "./load.sh 0 <.ko file name>- For Unloading the Module";;
 esac
