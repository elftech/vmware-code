#include <sys/utsname.h>
#include <unistd.h>

void main()
{
    struct utsname name;
    struct utsname *pname;
    pname=&name;
    if(uname(pname)<0){
	printf("Error!\n");
	exit(1);
    }
    printf("%s\n",pname->sysname);
    printf("%s\n",pname->nodename);
    printf("%s\n",pname->release);
    printf("%s\n",pname->version);
    printf("%s\n",pname->machine);
}
