#sum1 - print column sums
#input: rows of numbers
#output: sum of each column
#    missing entries are treated as zeros

	{for (i=1;i<=NF;i++)
		sum[i]+=$i
	 if(NF > maxfd)
		maxfld=NF
	 #print maxfld
 	}
END	{for(i=1;i<=maxfld;i++){
		#print sum[i]
		printf("%g",sum[i])
		if(i<maxfld)
			printf("\t")
		else
			printf("\n")
	 }
	}
