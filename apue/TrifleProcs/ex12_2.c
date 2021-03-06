//you can use env to look the difference in environ table
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
extern char **environ;

pthread_mutex_t env_mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void
thread_init(void)
{
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

int
putenv_r(const char *name)
{
	int i, len, olen;

//	char bufferIn[1024];
	//scanf("%s",bufferIn);
//	printf("%s\n",bufferIn);

	pthread_once(&init_done, thread_init);
	len = strlen(name);
	pthread_mutex_lock(&env_mutex);
	//we could't add the new string to the tail the table
	//And i don't know how the systme call putenv can do it
//	for (i = 0; environ[i] != NULL; i++) {
//		if ((strncmp(name, environ[i], len) == 0) &&
//		  (environ[i][len] == '=')) {
//			strcpy(&environ[i][len+1],bufferIn); 
//			pthread_mutex_unlock(&env_mutex);
//			return(0);
//		}
//	}
//	strcpy(&environ[i][0],name);
//	environ[i][len]='=';
//	strcpy(&environ[i][len+1],bufferIn);
        putenv(name);
	pthread_mutex_unlock(&env_mutex);
	return(ENOENT);
}

void *thr_fn(void *arg)
{
    int a=*((int *)arg);
    printf("thread %d is running:   %u   %u\n",a,(unsigned int)pthread_self(),getpid());
    putenv_r("WHATEVER=whatever");
    system("env|grep WHATEVER");
}

void main()
{
    int num[10]={1,2,3,4,5,6,7,8,9,10};
    int i;
    
    system("env|grep WHATEVER");
    for(i=0;i<5;++i){
        int err;
	pthread_t ntid;
	err=pthread_create(&ntid,NULL,thr_fn,num+i);
	if(err != 0)
	    err_quit("can't create thread:%s \n",strerror(err));
    }
    sleep(100);
}
