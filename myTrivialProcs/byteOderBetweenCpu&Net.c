#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

void main()
{
	printf("%d\n",htonl(47));
	printf("%d\n",ntohl(47));
}

