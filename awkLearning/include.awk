/^#include/ {
	gsub(/"/,"",$2)
	while(getline x<$2 >0)
		print x
	next
}
{	print	}
