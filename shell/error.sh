#!/bin/bash
#set -x 
#set -n
set -v
LIST="Peter Susan John Barry Lucy Norman Bill Leslie"
echo -n "Enter you Name: "
read NAME

for LOOP in $LIST
do
	if [ "$LOOP" = "$NAME" ];then
		echo "you're on the list,you're in"
		break
	fi
done

#set +x
#set +n
set +v
