#markov test: check that all words,pairs,triples in
# output ARGV[2] are in original input ARGV[1]
BEGIN {
	while(getline <ARGV[1]>0)
		for(i=1;i<=NF;i++){
			wd[++nw]=$i
			single[$i]++
		}
	for(i=1;i<nw;i++)
		pair[wd[i],wd[i+1]]++
	for(i=1;i<nw-1;i++)
		triple[wd[i],wd[i+1],wd[i+2]]++

	while(getline <ARGV[2]> 0){
		outwd[++ow]=$0
		if(!($0 in single))
			print "unexpect word",$0
	}
	for(i=1;i<ow;i++)
		if(!((outwd[i],outwd[i+1])in pair))
			print "unexpect pair",outwd[i],outwd[i+1]
	for(i=1;i<ow-1;i++)
		if(!((outwd[i],outwd[i+1],outwd[i+2]) in triple))
			print "unexpect triple",
				outwd[i],outwd[i+1],outwd[i+2]
}
