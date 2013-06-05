#!/bin/sh
FILES=`ls|grep [0-9]$`
for FF in $FILES
do 
mv $FF $FF.c
done
