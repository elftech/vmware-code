#!/bin/bash
#using if and then to select file or directory
#Binyuan 2011/4/13
if [ ! -e logical ]; then
      touch logical
      echo "Just make a file logical"
      exit 1
elif [ -e logical ] && [ -f logical ]; then
      rm logical
      mkdir logical
      echo "remove file ==> logical "
      echo "and make directory logical "
      exit 1
else
      echo "Does here have anything?"
fi
