//Usage: ./explicitLinkRunning /lib/../libm-...so
#include <stdio.h>
#include <dlfcn.h>

int main(int argc,int *argv[])
{
	void *handle;
	double (*func)(double);
	char *error;

	handle=dlopen(argv[1],RTLD_NOW);
	if(handle==NULL){
		printf("Open library %s error: %s\n",argv[1],dlerror());
		return -1;
	}

	func=dlsym(handle,"sin");
	if((error=dlerror())!=NULL){
		printf("Symbol sin not found: %s\n",error);
	}
	else
		printf("%f\n",func(3.141592657/2));

	dlclose(handle);
}


