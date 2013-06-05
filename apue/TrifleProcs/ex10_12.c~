#include "apue.h"

static void sig_alrm(int);

#define SIZE (1024*1*1024)

int main(void)
{
    long long n;
    char buf[SIZE];
    FILE *fp;

    fp=fopen("test.txt","wb");
    if(!fp)
	printf("Open error!\n");

    if(signal(SIGALRM,sig_alrm) == SIG_ERR )
	printf("signal(SIGALRM) error");

    alarm(1);

    if( (n=fwrite(buf, sizeof(char),SIZE,fp))<0)
	printf("write error!\n");
    printf("Writing completed!\n");
    exit(0);
}

static void
sig_alrm(int signo)
{
    printf("In signal handling!\n");
}
