//it doesn't work
#include <stdio.h>
#include <pthread.h>

int __attribute__ ((weakref)) pthread_create(
		pthread_attr_t *,
		void *(*)(void*),
		void *) ;

int main()
{
	if(pthread_create){
		printf("This is a multi-thread version!\n");
	}
	else 
		printf("This is a single-thread version!\n");
	
	return 0;
}

