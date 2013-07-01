#!/bin/bash

cat /etc/hosts|grep -v '^#'|awk 'system("ping -s -c1 "$1)'
