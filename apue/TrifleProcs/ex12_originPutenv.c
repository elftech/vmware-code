#include "stdio.h"

void main()
{
    char *buf="WHATEVER=whatever";
    putenv(buf);
    system("env|grep WHATEVER");
    char *buf1="WHATEVER";
    char *buf2="whatever";
    unsetenv(buf1);
}
