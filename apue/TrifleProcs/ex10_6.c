//This program should be compiled with 10_17
#include "apue.h"
#include <fcntl.h>

void main()
{
    int file,pid,buf,*pbuf;
    pbuf=&buf;
    buf=0;
    
    file=open("test.txt",O_RDWR|O_CREAT,S_IRUSR |S_IWUSR);
    write(file,&buf,1);
   
    TELL_WAIT();
    if((pid = fork())<0)
	printf("fork error");
    else if(pid == 0){
	++buf;
        printf("Child increments couter to %d\n",buf);
	while(1){
    	    TELL_PARENT(getppid());	
    	    WAIT_PARENT();
	
    	    buf+=2;
       	    printf("Child increments couter to %d\n",buf);
	    
	    TELL_WAIT();
	    sleep(1);
	}

	exit(0);
    }
    
    while(1){
    	WAIT_CHILD();
    
       	buf+=2;
	printf("Parent increments couter to %d\n",buf);

	TELL_WAIT();
	sleep(1);

	TELL_CHILD(pid);
    }

    exit(0);
}
