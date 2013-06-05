#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "3_5.c"

int main(void)
{
    int i,n;
    int fd[2];

    if(pipe(fd)<0)
	err_sys("pipe error");
    set_fl(fd[1],O_NONBLOCK);

    for(n=0;;++n)
	if((i=write(fd[1],"a",1))!=1){
	    printf("write ret %d, ",i);
	    break;
	}
    printf("pipe capacity = %d\n",n);
    exit(0);
}

