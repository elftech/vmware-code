#!/bin/bash -f
make clean
make
./sscull_load
echo "*************************************************************"
ls /dev/|grep sscull
echo "*************************************************************"
cat /proc/devices|grep sscull
./sscull_unload
make clean

