#!/bin/bash

trap "my_exit" 2
LOOP=0

my_exit()
{
	echo "You just hit <CTRL-C>,at number $LOOP"
	echo "I will now exit"
	exit 1
}

while :
do
	LOOP=`expr $LOOP + 1`
	echo $LOOP
done
