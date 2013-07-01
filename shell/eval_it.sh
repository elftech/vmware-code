#!/bin/sh
while read NAME TYPE
do
	eval `echo "${NAME}=${TYPE}"`
done < data

echo "You have a $PC pc,with a $MONITOR monitor"
echo "and are you network ? $NETWORK"
