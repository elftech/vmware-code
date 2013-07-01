#!/bin/sh
IFS=":"
while read rec1 rec2
do
	echo "First :$rec1"
	echo "Second :$rec2"
done < record.txt
