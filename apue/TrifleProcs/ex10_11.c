//This program should be compiled with 10_13
#include "apue.h"
#include <sys/resource.h>
#include <fcntl.h>

#define	BUFFSIZE        100	

void sig_xfsz(int signo)
{
    printf("In signal handling function!\n");
    exit(1);
}

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];
	struct rlimit lmt;
	int file,fd;
	int num;

	file=open("__init__.py",O_RDONLY,S_IRUSR);
	fd  =open("test_dest",O_RDWR|O_CREAT|O_TRUNC);

	lmt.rlim_cur=1024;
	lmt.rlim_max=RLIM_INFINITY;
	setrlimit(RLIMIT_FSIZE,&lmt);
	
	if(signal_intr(SIGXFSZ,sig_xfsz) == SIG_ERR)
	    printf("signal (SIGXFSZ) error");

	while ((n = read(file, buf, BUFFSIZE)) > 0){
	    num=write(fd, buf, BUFFSIZE);
	    printf("%d,%d\n",n,num);
    	    if (num != n)
		printf("write error\n");
	    else if (num != BUFFSIZE)
		printf("%d left was not writen!!",BUFFSIZE-num);
	}

	exit(0);
}
