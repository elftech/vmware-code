#BEGIN{ FS="\t"}
{
	for(i=1;i<=NF;i++)
		if(isnum($i))
	       		printf "true "
		else
			printf "false "
	printf "\n"
}	

function isnum(n) { return n ~ /^[+-]?[0-9]+$/ }
