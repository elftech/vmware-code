#include "apue.h"
#include <unistd.h>

void func()
{
    pid_t pid;
    if( (pid = vfork() ) <0)
	printf("fork error!\n");
    else if(pid == 0){
	printf("In child process!\n");
    }
    else
	printf("Back in called function after vfork\n");
}

int main(void)
{
    func();
    printf("Back in main!\n");
    return 0;
}

