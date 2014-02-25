#!/bin/bash -f
make clean
make
./scull_load
echo "*************************************************************"
ls /dev/|grep scull
echo "*************************************************************"
cat /proc/devices|grep scull
./scull_unload
make clean

