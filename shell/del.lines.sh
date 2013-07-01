#!/bin/bash
TEMP_F=del.lines.$$

usage()
{
	echo "Usage: `basename $0` file [file..]"
	echo "try `basename $0` -help for more info"
	exit 1
}

if [ $# -eq 0 ];then
	usage
fi

FILES=$1
while [ $# -gt 0 ]
do
	echo "..$1"
	case $1 in
		-help) `cat<<END
			Use this script to delete all blank lines from a text file(s)
			END`
			exit 0
			;;
		*)FILE_NAME=$1
			if [ -f $1 ];then
			awk '!/^$/' $FILE_NAME>$TEMP_F
			mv $TEMP_F $FILE_NAME
		else
			echo "`basename $0` cannot find this file : $1"
		fi
		shift
esac
done

