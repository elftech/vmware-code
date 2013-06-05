//This program runs after 7_3 has been compiled into echoarg
//And chmod +x echoarg
#include "apue.h"
#include <sys/wait.h>

int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0) {
		printf("fork error");
	} else if (pid == 0) {			/* child */
		if (execl("/home/tangsong/work/TrifleProcs/testinterp",
		//if (execl("/home/tangsong/work/TrifleProcs/echoall",
				  "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
			printf("execl error");
	}
	if (waitpid(pid, NULL, 0) < 0)	/* parent */
		printf("waitpid error");
	exit(0);
}
