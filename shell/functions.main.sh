#!/bin/sh

findit(){
	if [ $# -lt 1 ];then
		echo "usage: findit file"
		return 1
	fi
	find / -name $1 -print
}
