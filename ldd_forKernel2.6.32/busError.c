//
#include   <stdio.h> 
#include   <stdlib.h> 
int   main() 
{ 
    int   i[5]; 
    int   j; 
    i[0]=65536+2; 
    i[1]=65536*3+4; 
    j=*((int   *)((char   *)i+2));       /*   1   bus   error   */ 
    printf( "size   of   int   is   %d\nj=%d ",sizeof(int),j); 
    return   0; 
} 

