//The file should be compiled with 13_2 and 13_1 14_5 togother
#include <pthread.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h> //

sigset_t	mask;

extern int already_running(void);

void
reread(void)
{
	/* ... */
}

void *
thr_fn(void *arg) 
{
	int err, signo;

	for (;;) {
		err = sigwait(&mask, &signo);
		if (err != 0) {
			syslog(LOG_ERR, "sigwait failed");
			exit(1);
		}

		switch (signo) {
		case SIGHUP:
			syslog(LOG_INFO, "Re-reading configuration file");
			reread();
			break;

		case SIGTERM:
			syslog(LOG_INFO, "got SIGTERM; exiting");
			exit(0);

		default:
			syslog(LOG_INFO, "unexpected signal %d\n", signo);
		}
	}
	return(0);
}

int
main(int argc, char *argv[])
{
	int					err;
	pthread_t			tid;
	char				*cmd;
	struct sigaction	sa;

	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	/*
	 * Become a daemon.
	 */
	daemonize(cmd);

	/*
	 * Make sure only one copy of the daemon is running.
	 */
	if (already_running()) {
		syslog(LOG_ERR, "daemon already running");
		exit(1);
	}
	/*
	 * Restore SIGHUP default and block all signals.
	 */
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: can't restore SIGHUP default");
	sigfillset(&mask);
	if ((err = pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
		err_exit(err, "SIG_BLOCK error");

	/*
	 * Create a thread to handle SIGHUP and SIGTERM.
	 */
	err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err != 0)
		err_exit(err, "can't create thread");

	/*
	 * Proceed with the rest of the daemon.
	 */
	while(true){
	    int fd,fo;
	    fd=open("/dev/random",O_RDONLY);
	    char ch;
	    char str[5];
	    str[4]='\0';
	    int i;

//	    for(i=0;i<4;++i){
//		read(fd,str+i,1);
//		if(ch<0)
//		    ch=0;
//		ch='0'+ch;
//		str[i]=ch;
//	    }
            read(fd,str,5);

	    fo=open("/tmp/random",O_CREAT|O_APPEND|O_RDWR);
	    if(fo==-1)
		syslog(LOG_ERR,"can't create /tmp/random file\n");

	    if(write(fo,str,5)==-1)
		syslog(LOG_ERR,"can't write into /tmp/random file\n");
	    sleep(1);
	}
	/* ... */
	exit(0);
}
