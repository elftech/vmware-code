#include "apue.h"
#include <sys/wait.h>

//This program should be compiled with 8_3
int
main(void)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) < 0)
		printf("fork error");
	else if (pid == 0)				/* child */
		exit(7);

	if (wait(&status) != pid)		/* wait for child */
		printf("wait error");
	pr_exit(status);				/* and print its status */

	if ((pid = fork()) < 0)
		printf("fork error");
	else if (pid == 0)				/* child */
		abort();					/* generates SIGABRT */

	if (wait(&status) != pid)		/* wait for child */
		printf("wait error");
	pr_exit(status);				/* and print its status */

	if ((pid = fork()) < 0)
		printf("fork error");
	else if (pid == 0)				/* child */
		status /= 0;				/* divide by 0 generates SIGFPE */

	if (wait(&status) != pid)		/* wait for child */
		printf("wait error");
	pr_exit(status);				/* and print its status */

	exit(0);
}
