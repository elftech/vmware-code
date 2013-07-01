#!/bin/bash
MYDATE=`date +%d%m%y`
LOGFILE=backup_log.$MYDATE
>$LOGFILE

MYTIME=`date +%d%R`
LOGFILE2=admin_log.$MYTIME
>$LOGFILE2
