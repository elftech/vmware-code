#!/bin/bash
is_it_a_directory()
{
	_DIRECTORY_NAME=$1
	if [ $# -lt 1 ];then
		echo "is_it_a_directory: I need a directory name to check"
		return 1
	fi
	if [ ! -d $_DIRECTORY_NAME ];then
		return 1
	else 
		return 0
	fi
}
#-------------------------------------------------
error_msg()
{
	echo -e "\007"
	echo $@
	echo -e "\007"
	return 0
}

### END OF FUNCTIONS
echo -n "enter destination directory :"
read DIREC
if is_it_a_directory $DIREC
then :
else
	error_msg "$DIREC does not exist...creating it now"
	mkdir $DIREC >/dev/null 2>&1
	if [ $? != 0 ]
	then
		error_msg "Could not create directory:: check it out!"
		exit 1
	else :
	fi
fi

echo "extracting files...."

