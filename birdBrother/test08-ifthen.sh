#!/bin/bash
#set parameter in the if then
#需要加上hello这个参数才会显示正确!
#Binyuan 2011/4/12
if [ "$1" = "hello" ]; then
     echo "Hello!How are you?"
elif [ "$1" = "" ]; then
     echo "You MUST input parameters"
else
     echo "The only accept paremeter is Hello"
fi 
