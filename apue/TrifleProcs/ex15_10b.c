#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "3_clr_fl.c"

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define FIFO "temp.fifo"

int main(void)
{
    int fdread,fdwrite,n;
    unlink(FIFO);
    pid_t pid;

    if(mkfifo(FIFO,FILE_MODE)<0)
	err_sys("mkfifo error");
    if(fdread=open(FIFO,O_RDONLY|O_NONBLOCK)<0)
	err_sys("open error for reading ");
    if((fdwrite=open(FIFO,O_WRONLY))<0)
	err_sys("open error for writing ");
    clr_fl(fdread,O_NONBLOCK);
    
    char buf[8];
    n=write(fdwrite,"wwwwwww",8);
    if(n==-1)
	printf("can't write\n");
    
    if((pid = fork() )<0)
	err_sys("fork error");
    else if(pid == 0){
	sleep(1);
    	n=read(fdread,buf,8);
    	if(n==-1)
    	    printf("can't read\n");
	printf("%s",buf);

    	exit(0);
    }
}
