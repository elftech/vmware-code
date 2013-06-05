#include <time.h>

void main()
{
    char buf[100];
    int sz=100;
    time_t t;

    char *format="%G年 %m月  %d日 %A  %T %Z";
    time(&t);
    strftime(buf,sz,format,localtime(&t));
    printf("%s\n",buf);
}
