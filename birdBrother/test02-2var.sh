#!/bin/bash
#这个脚本的用途在于引用两个变量，顺便比较一下'与“的异同
#Date:2011/4/12
#Made by Binyuan
name="Binyuan"
myname1="My name is $name"
myname2='My name is $name'
echo $name
echo $myname1
echo $myname2
