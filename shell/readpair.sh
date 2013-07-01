#!/bin/sh
while read rec1
do
	read rec2
	echo "First :$rec1"
	echo "Second :$rec2"
done < record.txt
