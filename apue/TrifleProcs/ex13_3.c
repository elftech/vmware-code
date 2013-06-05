#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;
    char *p;

    daemonize("getlog");
    p=getlogin();
    fp=fopen("/tmp/getlog.out","w");
    if(fp!=NULL){
	if(p==NULL)
	    fprintf(fp,"no login name\n");
	else
	    fprintf(fp,"login name:%s\n",p);
    }
    else
	printf("the output file can't be created\n");
    exit(0);
}
