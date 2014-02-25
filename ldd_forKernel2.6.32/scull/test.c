#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#include <sys/types.h>
#define CSTR_LEN(a) (a+1)
int main()
{
    int fd,len;
    char inbuf[20]="tenacity";
    char outbuf[20];
    fd=open("/dev/scull",O_WRONLY);
    if(fd<=0){
	printf("Error openning the device of scull for writing!\n");
	exit(1);
    }
    len=write(fd,inbuf,CSTR_LEN(strlen(inbuf)));
    if(len<=0){
	printf("Error writing to the device!\n");
	close(fd);
	exit(1);
    }
    printf("writing %d bytes to the scull device!\n",len-1);
    close(fd);
    fd=open("/dev/scull",O_RDONLY);
    if(fd<=0){
	printf("Error openning the device of scull for reading!\n");
	exit(1);
    }
    len=read(fd,outbuf,CSTR_LEN(strlen(outbuf)));
    if(len<=0){
	printf("Error reading from the device!\n");
	close(fd);
	exit(1);
    }
    printf("reading %d bytes from the device!\n",len-1);
    printf("%s\n",outbuf);
}
