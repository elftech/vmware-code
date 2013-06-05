#include <linux/limits.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

static void
thread_init(void)
{
	pthread_key_create(&key, free);
}

char *
getenv(const char *name)
{
	int		i, len;
	char	*envbuf;

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&env_mutex);
	envbuf = (char *)pthread_getspecific(key);
	if (envbuf == NULL) {
		envbuf = malloc(ARG_MAX);
		if (envbuf == NULL) {
			pthread_mutex_unlock(&env_mutex);
			return(NULL);
		}
		pthread_setspecific(key, envbuf);
	}
	len = strlen(name);
	for (i = 0; environ[i] != NULL; i++) {
		if ((strncmp(name, environ[i], len) == 0) &&
		  (environ[i][len] == '=')) {
			strcpy(envbuf, &environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return(envbuf);
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return(NULL);
}

void *thr_fn(void *arg)
{
    int a=*((int *)arg);
    printf("thread %d is running:   %u   %u\n",a,pthread_self(),getpid());
    char *buf;
    buf=getenv("SHELL");
    printf("%s\n",buf);
}

void main()
{
    int num[10]={1,2,3,4,5,6,7,8,9,10};
    int i;
    for(i=0;i<5;++i){
        int err;
	pthread_t ntid;
	err=pthread_create(&ntid,NULL,thr_fn,num+i);
	if(err != 0)
	    err_quit("can't create thread:%s \n",strerror(err));
    }
    sleep(10);
}
