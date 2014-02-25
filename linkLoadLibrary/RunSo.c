//Usage: 
//./RunSo func_name arg1x arg2x ... x
//x: i-int,d-double,s-char *,v-void
//The program doesn't work.I guess there's somewhere wrong in the assembly code
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

void SETUP_STACK(int argc,char *argv[])
{
	int i=2;
	int esp=2;
	while(++i<argc-1){
		switch(argv[i][0]){
			case 'i':
				asm volatile("push %0"::
				"r"(atoi(&argv[i][1])));
				esp+=4;
				break;
			case 'd':
				atof(&argv[i][1]);
				asm volatile("subl $8,%esp\n"
				"fstpl (%esp)");
				esp+=8;
				break;
			case 's':
				asm volatile("push %0"::
				"r"(&argv[i][1]));
				esp+=4;
				break;
			default:
				printf("error argument type");
				exit(1);
		}
	}
}

#define RESTORE_STACK		\
	asm volatile("add %0,%%esp"::"r"(esp))

int main(int argc,char *argv[])
{
	void *handle;
	char *error;
	int i;
	int esp=0;
	void *func;

	handle=dlopen(argv[1],RTLD_NOW);
	if(handle==0){
		printf("Can't find library: %s\n",argv[1]);
		return -1;
	}

	func=dlsym(handle,argv[2]);
	if((error=dlerror())!=NULL){
		printf("Find symbol %s error: %s \n",argv[2],error);
		goto exit_runso;
	}

	switch(argv[argc-1][0]){
		case 'i':
			{
				int (*func_int)()=func;
				SETUP_STACK(argc,argv);
				int ret=func_int();
				RESTORE_STACK;
				printf("ret = %d\n",ret);
				break;
			}
		case 'd':
			{
				double (*func_double)()=func;
				SETUP_STACK(argc,argv);
				double ret=func_double();
				RESTORE_STACK;
				printf("ret = %f\n",ret);
				break;
			}
		case 's':
			{
				char *(*func_str)()=func;
				SETUP_STACK(argc,argv);
				char *ret=func_str();
				RESTORE_STACK;
				printf("ret = %s\n",ret);
				break;
			}
		case 'v':
			{
				void (*func_void)()=func;
				SETUP_STACK(argc,argv);
				func_void();
				RESTORE_STACK;
				printf("ret = void");
				break;
			}
	}

exit_runso:
	dlclose(handle);
}



