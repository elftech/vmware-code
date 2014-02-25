#!/bin/bash
#Usiing until
#Binyuan 2011/4/13

echo "Press Y/y to stop"
until [ "$yn" = "Y" ] || [ "$yn" = "y" ]
do 
    read yn
done 
echo "Stop here"
