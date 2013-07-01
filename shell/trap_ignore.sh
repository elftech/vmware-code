#!/bin/bash

trap "" 1 2 3 15

LOOP=0
my_exit()
{
	echo "Received interrupt on count $LOOP"
	echo "Now exiting..."
	exit 1
}

LOOP=0
while :
do
	LOOP=`expr $LOOP + 1`
	echo "critical processing..$LOOP..you cannot interrupt me"
	sleep 1
	if [ "$LOOP" -eq 6 ];then
		break
	fi
done

LOOP=0
trap "my_exit" 1 2 3 15
while :
do
	LOOP=`expr $LOOP + 1`
	echo "critical processing..$LOOP..you cannot interrupt me"
	sleep 1
	if [ "$LOOP" -eq 6 ];then
		break
	fi
done
