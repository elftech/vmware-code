#include <pthread.h>

struct msg {
	struct msg *m_next;
	/* ... more stuff here ... */
	int data;
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
		workq = mp->m_next;
		pthread_mutex_unlock(&qlock);
		/* now process the message mp */
		printf("%d\n",mp->data);
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

void main()
{
    int i;
    struct msg msgs[10];
    for(i=0;i<5;++i){
	msgs[i].data=i+1;
	enqueue_msg(msgs+i);
    }
    for(i=0;i<6;++i)
	process_msg();
}
