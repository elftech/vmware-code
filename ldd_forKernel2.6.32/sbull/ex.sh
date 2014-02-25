#!/bin/bash
make >/dev/null
./sbull_load
echo "*****************************************************"
ls /dev/|grep sbull
echo "*****************************************************"
cat /proc/devices|grep sbull
echo "*****************************************************"
./sbull_unload
make clean
