#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct msg {
	struct msg *m_next;
	/* ... more stuff here ... */
	// 用于保存msg宿主上的数据1
	void *private_data;
};

struct job {
	struct job *j_next;
	struct job *j_prev;
	pthread_t   j_id;   /* tells which thread handles this job */
	/* ... more stuff here ... */
};

struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void
process_msg(void)
{
	struct msg *mp;

	for (;;) {
		pthread_mutex_lock(&qlock);
		while (workq == NULL)
			pthread_cond_wait(&qready, &qlock);
		mp = workq;
		if(((struct job *)mp->private_data)->j_id==pthread_self()){
		    workq = mp->m_next;
		}
		pthread_mutex_unlock(&qlock);
		/* now process the message mp */
		//printf("%d\n",mp->data);
		printf("Thread %u is running\n",(unsigned int)pthread_self());
		if(((struct job *)mp->private_data)->j_id!=pthread_self()){
		    printf("Thread %u didn't match jobId %u\n",(unsigned int)pthread_self(),(unsigned int)((struct job*)mp->private_data)->j_id);
		    sleep(1);
		    continue;
		}
		else 
    		    printf("Thread %u is handling messages\n",(unsigned int)pthread_self());
		//让其他的进程也用机会处理消息
		sleep(1);
	}
}

void
enqueue_msg(struct msg *mp)
{
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	pthread_mutex_unlock(&qlock);
	pthread_cond_signal(&qready);
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

struct queue {
	struct job      *q_head;
	struct job      *q_tail;
	pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue.
 */
int
queue_init(struct queue *qp)
{
	int err;

	qp->q_head = NULL;
	qp->q_tail = NULL;
	err = pthread_rwlock_init(&qp->q_lock, NULL);
	if (err != 0)
		return(err);

	/* ... continue initialization ... */

	return(0);
}

/*
 * Insert a job at the head of the queue.
 */
void
job_insert(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = qp->q_head;
	jp->j_prev = NULL;
	if (qp->q_head != NULL)
		qp->q_head->j_prev = jp;
	else
		qp->q_tail = jp;	/* list was empty */
	qp->q_head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
        //@Binyuan
	struct msg *mp=(struct msg *)malloc(sizeof(struct msg));
	mp->private_data=(void *)jp;
	enqueue_msg(mp);
}

/*
 * Append a job on the tail of the queue.
 */
void
job_append(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = NULL;
	jp->j_prev = qp->q_tail;
	if (qp->q_tail != NULL)
		qp->q_tail->j_next = jp;
	else
		qp->q_head = jp;	/* list was empty */
	qp->q_tail = jp;
	pthread_rwlock_unlock(&qp->q_lock);
        //@Binyuan
	struct msg *mp=(struct msg *)malloc(sizeof(struct msg));
	mp->private_data=(void *)jp;
	enqueue_msg(mp);
}

/*
 * Remove the given job from a queue.
 */
void
job_remove(struct queue *qp, struct job *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	if (jp == qp->q_head) {
		qp->q_head = jp->j_next;
		if (qp->q_tail == jp)
			qp->q_tail = NULL;
	} else if (jp == qp->q_tail) {
		qp->q_tail = jp->j_prev;
		if (qp->q_head == jp)
			qp->q_head = NULL;
	} else {
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}
	pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Find a job for the given thread ID.
 */
struct job *
job_find(struct queue *qp, pthread_t id)
{
	struct job *jp;

	if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
		return(NULL);

	for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
		if (pthread_equal(jp->j_id, id))
			break;

	pthread_rwlock_unlock(&qp->q_lock);
	return(jp);
}

void *thr_fn(void *arg)
{
    int a=*((int *)arg);
    printf("thread %d is waiting:   %u   %u\n",a,(unsigned int)pthread_self(),(unsigned int)getpid());
    process_msg();
    printf("thread %d has done:   %u   %u\n",a,(unsigned int)pthread_self(),(unsigned int)getpid());
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define THREAD_NUM 3
void main()
{

    int i;
    int num[10]={1,2,3,4,5,6,7,8,9,10};
    int ntid[10];
    for(i=0;i<THREAD_NUM ;++i){
	int err;
	err=pthread_create((pthread_t *)(ntid+i),NULL,thr_fn,num+i);
	if(err != 0)
	    err_quit("can't create thread:%s \n",strerror(err));

    }
    sleep(1);

    struct queue Q;
    queue_init(&Q);
    for(i=0;i<10;++i){
	struct job *pj;
	pj=(struct job *)malloc(sizeof(struct job));
	pj->j_id=ntid[i%THREAD_NUM];
	job_insert(&Q,pj);
    }
    sleep(10);

}
