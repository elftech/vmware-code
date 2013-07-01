#!/bin/sh
#BUG: read doesn't stop when encouters the end of file 
FILENAME=myfile.txt
FILENAME_BAK=myfile.bak
if [ -s $FILENAME ];then
	exec 4>$FILENAME_BAK
	exec 3<$FILENAME

	while :
	do
		read LINE <&3
		echo $LINE
		if [ "$?" -ne 0 ];then
			exec 3<$-
			exec 4<$-
			exit
		fi
		echo $LINE>&4
	done
else
	echo "`basename $0` : Sorry,$FILENAME is not present or is empty" >&2
fi
