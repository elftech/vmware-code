#!/bin/bash

ALL=false
HELP=false
FILE=false
VERBOSE=false
COPIES=0

#while getopts :ahfgvpc: OPT
while getopts ahfgvpc: OPT
do
	case $OPT in
		a)ALL=true
			echo "All is $ALL"
			;;
		h)HELP=true
			echo "Help is $HELP"
			;;
		f)FILE=true
			echo "File is $FILE"
			;;
		v)VERBOSE=true
			echo "Verbose is $VERBOSE"
			;;
		p)
			echo "Nothing"
			;;
		c)
			COPIES=$OPTARG
			echo "COPIES is $COPIES"
			;;
		\?)
			echo "`basename $0` -[a h f v] -[c value] file"
			;;
	esac
done
