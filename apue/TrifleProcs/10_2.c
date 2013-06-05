#include "apue.h"
#include <pwd.h>

static void
my_alarm(int signo)
{
	struct passwd	*rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL)
    	    printf("getpwnam(root) error");
	printf("%s\n",rootptr->pw_name);
	printf("%d\n",alarm(1));
}

int
main(void)
{
	struct passwd	*ptr;
	int i,j;

	signal(SIGALRM, my_alarm);
	alarm(1);
	for (/*i=0,j=i ; ;++i*/ ;;) {
		if ((ptr = getpwnam("tangsong")) == NULL)
			printf("getpwnam error");
		if (strcmp(ptr->pw_name, "tangsong") != 0)
			printf("return value corrupted!, pw_name = %s\n",
					ptr->pw_name);
	printf("%s\n",ptr->pw_name);
/*		if(i-j>=1000){
    		    printf("%d\n",i);
		    j=i;
		}*/
	}
}
