#include	"../lib/unp.h"

void
str_echo(int sockfd)
{
	char		line[MAXLINE];
	FILE		*fpin, *fpout;

	fpin = Fdopen(sockfd, "r");
	fpout = Fdopen(sockfd, "w");

	setvbuf(fpout,NULL,_IOLBF,0);

	while (Fgets(line, MAXLINE, fpin) != NULL)
		Fputs(line, fpout);
}