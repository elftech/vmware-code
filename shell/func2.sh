#!/bin/sh
#func2
char_name()
{
	_LETTERS_ONLY=$1
	_LETTERS_ONLY=`echo $1|awk '{if($0~/^[a-zA-Z]+$/) print "1"}'`
	if [ "$_LETTERS_ONLY" != "" ]
	then
		return 0
	else
		return 1
	fi
}

name_error()
{
	echo "$@ contains error, it must contain only letters"
}

while :
do
	echo -n "What is your first name :"
	read F_NAME	
	if char_name $F_NAME
	then 
		break
	else
		name_error $F_NAME
	fi
done

while :
do
	echo -n "What is your surname :"
	read S_NAME
	if char_name $S_NAME
	then
		break
	else
		name_error $S_NAME
	fi
done

