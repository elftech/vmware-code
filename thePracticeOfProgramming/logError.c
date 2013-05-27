#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

/* errno main: test errno */
int main(void)
{
	double f;
	errno=0;
	f=log(-1.23);
	printf("%f %d %s\n",f,errno,strerror(errno));
	return 0;
}
