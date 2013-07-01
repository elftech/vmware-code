#!/bin/sh
#BUG:Errors in using 'cat' in `..`
MYDATE=`date +%d/%m/%Y`
THIS_HOST=`hostname -s`
USER=`whoami`

while :
do
	tput clear

	`cat <<END
	___________________________________________________

	User: $USER      Host: $THIS_HOST   Date: $MYDATE
	___________________________________________________

		1 : List files in current directory
		2 : Use the vi editor
		3 : See who is on the system
		H : Help screen
		Q : Exit Menu
	___________________________________________________
	END`	

	echo -e -n "\t Your choice [1,2,3,H,Q] >"
	read CHOICE
	case $CHOICE in
		1)ls
			;;
		2)vi
			;;
		3) who
			;;
		H|h)
			`cat<<END
			This is the help screen,nothing yet to help you!
			END`
			;;
		Q|q) exit 0
		;;
	*) echo -e "\t\007 unknow user response"
		;;
esac

echo -e -n "\t Hit the return key to continue"
read DUMMY
done

		
