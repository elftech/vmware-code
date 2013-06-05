#include <stdio.h>
#include <unistd.h>

void main()
{
    int i=1,j=0,fd;
    //printf("hello world\n");
    write(1,"hello world \n",13);
    write(2,"Error output\n",13);
    i=i/j;
} 
