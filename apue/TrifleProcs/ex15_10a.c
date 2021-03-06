#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

void test(int fd)
{
   char buf[100];
   int nread;
   printf("Testing\n");

   while(1){
       if((nread = read(fd,buf,100)) ==-1){
	   if(errno == EAGAIN){
	       printf("no data\n");
	       return;
	   }
       }
       
       buf[nread]='\0';
       printf("the data of %d bytes read from FIFO is:%s\n",nread,buf);
       sleep(1);
   }
}

void main()
{
    int fdr,fdw,fdrw,n,fdrw2;
    mkfifo("fifor",O_RDONLY);
    mkfifo("fifow",O_WRONLY);
    mkfifo("fiforw",O_RDONLY|O_WRONLY);

    fdr=open("fifor",O_RDONLY|O_NONBLOCK);
    fdw=open("fifow",O_WRONLY|O_NONBLOCK); 
    fdrw=open("fiforw",O_WRONLY|O_NONBLOCK|O_RDONLY); 

    if( (n=write(fdr,"the read file can be writen to !\n",100))==-1)
	printf("error,can't write to read file\n");
    n=write(fdw,"the write file can be writen to !\n",100);
    if(n==-1)
	printf("error,can't write to write file\n");

    n=write(fdrw,"the readi-write file can be writen to !\n",100);
    if(n==-1)
	printf("error,can't write to read-write file\n");
    
    //test(fdr);
    //test(fdw);
    //test(fdrw);
}
