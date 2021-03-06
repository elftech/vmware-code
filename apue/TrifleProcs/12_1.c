#include "apue.h"
#include <pthread.h>

char *str="Yeah,we can get exited status.";
int
makethread(void *(*fn)(void *), void *arg,pthread_t *ptid)
{
	int				err;
	pthread_attr_t	attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
		return(err);
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0)
		err = pthread_create(ptid, &attr, fn, arg);
	pthread_attr_destroy(&attr);
	return(err);
}

void *thr_fn(void *arg)
{
    pthread_exit((void *)str);
}
//
//void main()
//{
//    int err;
//    pthread_t ntid;
//    void *tret;
//    makethread(thr_fn,NULL,&ntid);
//    err=pthread_join(ntid,&tret);
//    if(err != 0)
//	err_quit("can't join with thread: %s\n",strerror(err));
//    printf("The exited status is : %s\n",(char *)tret);
//}
//
