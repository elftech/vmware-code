#!/bin/bash -f
make clean >/dev/null
make >/dev/null
insmod usb-skeleton.ko
echo "*************************************************************"
ls /dev/|grep usb
echo "*************************************************************"
cat /proc/devices|grep usb
echo "*************************************************************"
tail -5 /var/log/messages
dmesg |tail -5
echo "============================================================="
rmmod usb-skeleton.ko
echo "*************************************************************"
ls /dev/|grep usb
echo "*************************************************************"
cat /proc/devices|grep usb
echo "*************************************************************"
tail -5 /var/log/messages
dmesg |tail -5
make clean >/dev/null

