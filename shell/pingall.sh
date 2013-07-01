#!/bin/bash

cat /etc/hosts|grep -v '^#'|while read LINE
do
	ADDR=`awk '{print $1}' `
	echo $ADDR
	for MACHINE in $ADDR
	do
		ping -s -c1 $MACHINE
	done
done
