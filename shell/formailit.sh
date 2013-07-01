#!/bin/sh
WHOS_ON=`who -u|awk '{ print $1 } '`
for user in $WHOS_ON
do
	`mail $user <<END
	Dear $user,
	fuck you!
	$LOGNAME
	END`
done
