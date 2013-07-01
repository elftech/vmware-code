#!/bin/sh
#ifmkdir 
DIRECTORY=$1

if [ "$DIRECTORY" = "" ]
then
	echo "Usage : `basename $0` direcotory to create" >$2
	exit 1
fi

if [ -d $DIRECTORY ]
then :
else
	echo "The direcotory does not exist"
	echo -n "Create it now? [y..n] :"
	read ANS
	if [ "$ANS" = "y" ] || [ "$ANS" = "Y" ]
	then
		echo "creating now"
		mkdir $DIRECTORY >/dev/null 2>$1
		if [ $? != 0 ];then
			echo "Error creating the direcotory $DIRECTORY" >&2
		       exit 1
	       fi
       else :
       fi
fi

