#include <stdlib.h>
#include <pthread.h>

struct job {
	struct job *j_next;
	struct job *j_prev;
	pthread_t   j_id;   /* tells which thread handles this job */
	/* ... more stuff here ... */
};

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
    printf("thread %d is running:   %u   %u\n",a,pthread_self(),getpid());
    while(1){};
}

void main()
{
    struct queue Q;
    int i;
    int num[10]={1,2,3,4,5,6,7,8,9,10};
    queue_init(&Q);
    for(i=0;i<5;++i){
	struct job *pj;
	int err,ntid;
	pj=(struct job *)malloc(sizeof(struct job));
	err=pthread_create(&ntid,NULL,thr_fn,num+i);
	if(err != 0)
	    err_quit("can't create thread:%s \n",strerror(err));
	pj->j_id=ntid;
	job_insert(&Q,pj);
    }
    for(i=5;i<10;++i){
	struct job *pj;
	int err,ntid;
	pj=(struct job *)malloc(sizeof(struct job));
	err=pthread_create(&ntid,NULL,thr_fn,num+i);
	if(err != 0)
	    err_quit("can't create thread:%s \n",strerror(err));
	pj->j_id=ntid;
	job_append(&Q,pj);
    }
    sleep(2);

}
