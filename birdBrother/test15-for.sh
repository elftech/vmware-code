#!/bin/bash
#Using for and loop to read the account of this linux server!
#Binyuan 2011/4/13
account=`cut -d ":" -f1 /etc/passwd|sort`
echo "The following is your linux server's accout"
echo "Fist:Unorganized:"
echo $account
echo ""
echo "Second:Organzide:"
for i in $account
do 
   echo $i
done

 
