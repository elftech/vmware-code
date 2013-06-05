#include "apue.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void
pr_mask(const char *str)
{
    sigset_t sigset;
    int      errno_save,i,pos;
    FILE     *fp;
    char     name[50];
    char     line[100];

    errno_save=errno;
    if(sigprocmask(0,NULL,&sigset)<0)
	printf("sigprocmask error");

    printf("%s\n",str);
    fp=fopen("signal.h","r");
    while(1){
	char def[8],sig[4];

	fgets(line,100,fp);
	strncpy(def,line,8);
	def[7]='\0';
	
	if(strcmp(def,"#define")!=0)
	    continue;
	for(i=8;line[i]==' ';++i);
	pos=i;

        strncpy(sig,&line[i],4);	
	sig[3]='\0';

	if(strcmp(sig,"SIG")!=0)
	    continue;
	break;
    }
     
    for(i=0;i<32;++i){
	if(sigismember(&sigset,1<<i)){
            strcpy(name,&line[pos]);
	    printf("%s",name);
	}
	fgets(line,100,fp);
    }
}

void main()
{
    pr_mask("Hello,let's show it");
}
