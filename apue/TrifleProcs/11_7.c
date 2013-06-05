#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define NHASH 29
#define HASH(fp) (((unsigned long)fp)%NHASH)

int count;
struct foo *m_fp;

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int             f_count; /* protected by hashlock */
	pthread_mutex_t f_lock;
	struct foo     *f_next; /* protected by hashlock */
	int             f_id;
	/* ... more stuff here ... */
};

struct foo *
foo_alloc(void) /* allocate the object */
{
	struct foo	*fp;
	int			idx;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
			free(fp);
			return(NULL);
		}
		idx = HASH(fp);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[idx];
		fh[idx] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);
		/* ... continue initialization ... */
	}
	return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
	pthread_mutex_lock(&hashlock);
	fp->f_count++;
	pthread_mutex_unlock(&hashlock);
}

struct foo *
foo_find(int id) /* find a existing object */
{
	struct foo	*fp;
	int			idx;

	idx = HASH(fp);
	pthread_mutex_lock(&hashlock);
	for (fp = fh[idx]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			fp->f_count++;
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return(fp);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
	struct foo	*tfp;
	int			idx;

	pthread_mutex_lock(&hashlock);
	if (--fp->f_count == 0) { /* last reference, remove from list */
		idx = HASH(fp);
		tfp = fh[idx];
		if (tfp == fp) {
			fh[idx] = fp->f_next;
		} else {
			while (tfp->f_next != fp)
				tfp = tfp->f_next;
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
		//we can use fp as an indicator
		m_fp=NULL;
		printf("fp is free!!!\n");
	} else {
		pthread_mutex_unlock(&hashlock);
	}
}

void *thr_fn(void *arg)
{
    while(m_fp!=NULL){
	foo_hold(m_fp);
	printf("thread %u is using foo.\n",(unsigned int)pthread_self());
	sleep(1);
    }
    printf("thread %u has no foo to use.\n",(unsigned int)pthread_self());
}

void main()
{
    int i,err;
    unsigned int ntid;
    //Intial our hash
    for(i=0;i<NHASH;++i)
	fh[i]=NULL;
    m_fp=foo_alloc();
    for(i=0;i<5;++i){
	err=pthread_create((pthread_t *)(&ntid),NULL,thr_fn,NULL);
	if(err!=0)
	    err_quit("can't create thread:%s \n",strerror(err));
    }
    sleep(5);
    while(m_fp!=NULL){
	printf("usage count: %d\n",m_fp->f_count);
	foo_rele(m_fp);
    }
    printf("usage count: 0\n");
    sleep(10);
}
