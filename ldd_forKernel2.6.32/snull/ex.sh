#!/bin/bash
HOSTFILE="/etc/hosts"
NTWKFILE="/etc/networks"
LEN_HOSTFILE=$(cat $HOSTFILE|wc -l) 
LEN_NTWKFILE=$(cat $NTWKFILE|wc -l)

make >/dev/null
./snull_load
echo "**************to see if snull in /dev/***************"
ls /dev/|grep snull
echo "***to see snull's device number in /proc/devices*****"
cat /proc/devices|grep sn
echo "***to see if sn%d are interfaces of net already******"
ifconfig -a|grep 'sn'
#设置IP宏
echo "
      
192.168.0.88             local0
192.168.0.99             remote0
192.168.1.99             local1
192.168.1.88             remote1">>$HOSTFILE
echo "

snullnet0             192.168.0.0         
snullnet1             192.168.1.0">>$NTWKFILE
#设置掩码
echo "*****************add subnet masking*******************"
#是c类地址，所以不用加掩码
#route add -net snullnet0 netmask 255.255.255.0 sn0
#route add -net snullnet1 netmask 255.255.255.0 sn1
#设置接口
echo "*****************ifconfig sn0*************************"
ifconfig sn0
echo "*****************ifconfig sn1*************************"
ifconfig sn1
#测试
echo "*****************ping -c 1 remote0********************"
ping -c 2 remote0
echo "*****************ping -c 1 remote1********************"
ping -c 2 remote0

#清理工作
#LEN_HOSTFILE=$(expr $(cat $HOSTFILE|wc -l) - 6) #事先保存其值就不要这么复杂地计算了
#LEN_NTWKFILE=$(expr $(cat $NTWKFILE|wc -l) - 3)

head  -$LEN_HOSTFILE $HOSTFILE>tmpfile
mv tmpfile $HOSTFILE
head  -$LEN_NTWKFILE $NTWKFILE>tmpfile
mv tmpfile $NTWKFILE

./snull_unload
make clean>/dev/null
