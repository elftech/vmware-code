//This program should be compiled with 8_3,8_12
#include "apue.h"
#include <sys/wait.h>

#include "8_3.c"

int
main(void)
{
	int		status;

	if ((status = system("date")) < 0)
		printf("system() error");
	pr_exit(status);

	if ((status = system("nosuchcommand")) < 0)
		printf("system() error");
	pr_exit(status);

	if ((status = system("who; exit 44")) < 0)
		printf("system() error");
	pr_exit(status);

	exit(0);
}
