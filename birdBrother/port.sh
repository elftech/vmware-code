#!/bin/bash
#program:Using to study the if-then program
#Made by:Binyuan
#date:2011/4/12
#content:I will use this program to show your services
#1.print the program's work in the screen
echo "Now,the services of your linux system will be detect!"
echo "The www,ftp,ssh,and sendmail + pop3 will be detect!"
echo " "
#2.www
www='netstat -an|grep LISTEN|grep :80'
if [ "$www" != "" ]; then
     echo "WWW is running "
else
     echo "WWW is NOT running"
fi 
#3.ftp
ftp='netstat -an|grep LISTEN|grep :21'
if [ "$ftp" != "" ]; then
     echo "FTP is running" 
else 
     echo "FTP is NOT running"
fi
