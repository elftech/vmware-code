#! /bin/sh
exec awk '
NR == 1{
     n=NF
     for(i=0;i<=NF;i++)
	 row[i]=$i
     next
 }

 {
     if(NF>n)
	 n=NF
     for(i=0;i<=NF;i++)
	 row[i]=row[i] " " $i
     } 
END {
     for(i=1;i<=n;i++)
	 print row[i]
}' ${1+"$@"}
