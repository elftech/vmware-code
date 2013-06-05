#include "fcntl.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include <signal.h>
#include <poll.h>

#define MAXLINE 1024

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
}

void testReslt(short ret)
{
    switch(ret){
    case POLLERR:
	printf("POLLERR\n");
	break;
    case POLLHUP:
	printf("POLLHUP\n");
	break;
    case POLLNVAL:
	printf("NO FILE FOR THE DESCRIPTOR\n");
	break;
    case POLLIN:
	printf("POLLIN\n");
    case POLLOUT:
	printf("POLLOUT\n");
    default:
	printf("Others\n");
    }
}

void test1(int *fd)
{
    struct timeval tm;
    fd_set readfds,writefds,exceptfds;
    int ret;

    tm.tv_sec=5;
    tm.tv_usec=0;

    FD_ZERO(&readfds);
    FD_SET(fd[0],&readfds);
    if(FD_ISSET(fd[0],&readfds))
	printf("The bit is set\n");
    
    ret=select(fd[0]+1,&readfds,NULL,NULL,&tm);
    if(ret == -1)
	printf("Error:probably a signal happens\n");
    else if(ret == 0)
	printf("No read descriptor ready\n");
    else 
	printf("There are %d descriptors ready\n",ret);
    if(FD_ISSET(fd[0],&readfds))
	return;
    else 
	printf("We can't read\n");
}

void test2(int *fd)
{
    struct pollfd plfd[3];
    int ret;
    int i;
    plfd[0].fd=fd[0];
    plfd[0].events=POLLIN;
    plfd[1].fd=fd[1];
    plfd[1].events=POLLOUT;
    plfd[2].fd=FD_SETSIZE;
    plfd[2].events=POLLOUT;

    ret=poll(plfd,3,0);

    for( i=0;i<3;++i)
	testReslt(plfd[i].revents);
//    if(plfd[0].revents==POLLHUP)
//	printf("The read has been suspended\n");
//    if(plfd[1].revents==POLLHUP)
//	printf("The write has been suspended\n");
//    else if(plfd[1].revents==POLLERR)
//	printf("can't write\n");
//    if(plfd[2].revents==POLLNVAL)
//	printf("no file for this descriptor\n");
//
    printf("There are %d descirptors ready\n",ret);
}

int
main(void)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	if(signal(SIGPIPE,sig_pipe) == SIG_ERR)
	    err_sys("signal error");

	if (pipe(fd) < 0)
		err_sys("pipe error");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {		/* parent */
	        //sleep(1);
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
		printf("IN PARENT:---------------------------------\n");
		test2(fd);
		close(fd[1]);
	} else {				/* child */
	        sleep(1);
		close(fd[0]);
		close(fd[1]);
		printf("IN CHILD:----------------------------------\n");
		test1(fd);
		n = read(fd[0], line, MAXLINE);
		//close(fd[0]);
		write(STDOUT_FILENO, line, n);
	}
	exit(0);
}
