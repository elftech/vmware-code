#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "apue.h"
#include "loopQueue.h"

//We asume that there is a list-sorted queue that can store timers 
//
void multiAlarm(int i);

static void sig_alrm(int signo)
{
    int unsleep;
    printf("In sig_alrm!!\n");
    unsleep=alarm(0);
    printf("There are %d seconds left!\n",unsleep);
    if(!unsleep){
	printf("%d secs left!",unsleep);
	multiAlarm(unsleep);
    }
}

void multiAlarm(int i)
{
    struct sigaction newact,oldact;

    newact.sa_handler =sig_alrm;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags=0;
    sigaction(SIGALRM,&newact,&oldact);
    
//    if(!alarm(0))
 //   	sigaction(SIGALRM,&oldact,NULL);
}

void main()
{
    int i;
   for(i=1;i<=5;++i)
    	multiAlarm(i);
    while(1)
    	pause();
}
