#!/bin/bash -f
make
insmod registerDevice.ko
make clean
cat /proc/devices|grep BB
cat /proc/devices|grep CC
rmmod registerDevice.ko
echo "****************************************"
cat /proc/devices|grep BB
cat /proc/devices|grep CC
