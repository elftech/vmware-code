#include "apue.h"

extern void daemonize(const char *cmd);

void main()
{
    daemonize("whatever");
    kill(getpid(),SIGTSTP);
    printf("I have been revived!\n");
}
