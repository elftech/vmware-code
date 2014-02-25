#!/bin/bash
#This program is used to study if then
#Binyuan 2011/4/12
echo "Press y to continue"
read yn
if [ "$yn" = "y" ] || [ "$yn" = "Y" ]; then
     echo "script is running..."
else
     echo "STOP!"
fi
