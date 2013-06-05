#include "apue.h"
//This program should be compiled with 10_17

static void charatatime(char *);

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if( (pid = fork() ) <0)
	printf("fork error");
    else if (pid == 0){
	charatatime("output from child\n");
	TELL_PARENT(getppid());
    }
    else{
	WAIT_CHILD();
	charatatime("output from parent\n");
    }
}

static void
charatatime(char *str)
{
	char	*ptr;
	int		c;

	setbuf(stdout, NULL);			/* set unbuffered */
	for (ptr = str; (c = *ptr++) != 0; )
		putc(c, stdout);
}
