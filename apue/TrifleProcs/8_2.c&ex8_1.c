#include "apue.h"
#include <unistd.h>

int		glob = 6;		/* external variable in initialized data */

int
main(void)
{
	int	var,i;		/* automatic variable on the stack */
	char    buf[10];
	pid_t	pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdio */
	if ((pid = vfork()) < 0) {
		printf("vfork error");
	} else if (pid == 0) {		/* child */
		glob++;					/* modify parent's variables */
		var++;
//		_exit(0);				/* child terminates */
                fclose(stdout);
		exit(0);
	}

	/*
	 * Parent continues here.
	 */
//	close(STDOUT_FILENO);
	i=printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	sprintf(buf,"%d\n",i);
	write(STDOUT_FILENO,buf,strlen(buf));

	exit(0);
}
