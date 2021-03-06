BEGIN {
	FS = ":" }
NF !=7 {
	printf("line %d,does not have 7 fields: %s\n",NR,$0) }

$1 ~ /[^A-Za-z0-9]/ {
	printf("line %d,nonalphanumeric user id: %s\n",NR,$0) }

$2 == "" {
	printf("line %d, no passwd: %s\n",NR,$0) }

$3 ~ /[^0-9]/ {
	printf("line %d,nonnumberic user id: %s\n",NR,$0)  }

$4 ~ /[^0-9]/ {
	printf("line %d,nonnumeric group id: %s\n",NR,$0) }

$6 !~ /^\// {
	printf("line %d,invalid login directory: %s\n",NR,$0) }
