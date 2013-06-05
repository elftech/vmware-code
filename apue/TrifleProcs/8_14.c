//This program should be compiled with 8_3,8_12,or with 8_3and 10_20
#include "apue.h"

int
main(int argc, char *argv[])
{
	int		status;

	if (argc < 2)
		printf("command-line argument required");

	if ((status = system(argv[1])) < 0)
		printf("system() error");
	pr_exit(status);

	exit(0);
}
