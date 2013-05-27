#include <stdio.h>

#define ALIGN(P,ALIGNBYTES) ((void *)( ((unsigned long)P+ALIGNBYTES-1)&~(ALIGNBYTES-1)))

void main()
{
	printf("%d\n",ALIGN(0,16)); 
	printf("%d\n",ALIGN(64,16)); 
	printf("%d\n",ALIGN(13,16)); 
	printf("%d\n",ALIGN(13,32));
	printf("%d\n",ALIGN(26,16));
	printf("%d\n",ALIGN(26,15));
}
