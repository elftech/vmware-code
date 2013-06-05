#include "apue.h"
#include <setjmp.h>

jmp_buf buf;
jmp_buf buf2;

void handler(int s)
{
    if(s == SIGINT )
	printf("Now got a SIGINT signal\n");
}

void handler2(int s)
{
    if(s == SIGSEGV )
	printf("Now got a SIGSEGV signal\n");
}

int main(int argc,char *argv[])
{
    signal(SIGINT,handler);
    signal(SIGSEGV,handler2);
    printf("OK here!\n");

    if(setjmp(buf)){
	printf("back in main!");
	return 0;
    }
    else
	printf("first time through\n");

    if(setjmp(buf2))
	printf("back in main\n");
    else{
	int *p=NULL;
	*p=0;
    }

    while(1);
    return 0;
}
