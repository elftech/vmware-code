/*===============================================================

 * File Name : producerConsumerProblem.c
 * Creation Date : 2013-04-11
 * Last Modified : 2013年04月11日 星期四 20时53分13秒
 * Create By : Binyuan
 * Email : elf.ggle@gmail.com
 * Purpose :test linux semaphore usage

================================================================*/
#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

void *producer_handler(void *ptr);
void *consumer_handler(void *ptr);

sem_t mutex,blank,fill;
int *buffer;
int in=0,out=0,buffer_size=10;

void main()
{
	if((buffer=(int *)malloc(buffer_size*sizeof(int)))==NULL)
		printf("can't allocate memroy on heap\n");
	sem_init(&mutex,0,1);
	sem_init(&blank,0,buffer_size);
	sem_init(&fill,0,0);

	int err;
	pthread_t producer,consumer;
	err=pthread_create(&producer,NULL,producer_handler,NULL);
	if(err!=0)
		err_quit("can't create thread: %s\n",strerror(err));
	err=pthread_create(&consumer,NULL,consumer_handler,NULL);
	if(err!=0)
		err_quit("can't create thread: %s\n",strerror(err));

	sleep(10);

}

void *producer_handler(void *ptr)
{
	while(1){
		static int data=0;
		sem_wait(&blank);
	//	sem_wait(&mutex);
	
		buffer[in]=++data;
		printf("%d has been input to the buffer\n",data);
		in=(in+1+buffer_size)%buffer_size;
	//	sem_post(&mutex);
		sem_post(&fill);
		sleep(1);
	}
	return ((void *)0);
}

void *consumer_handler(void *ptr)
{
	while(1){
		int fetch;
		sem_wait(&fill);
	//	sem_wait(&mutex);
		fetch=buffer[out];
		out=(out+1+buffer_size)%buffer_size;
		printf("%d has been fetched\n",fetch);
	//	sem_post(&mutex);
		sem_post(&blank);
		sleep(2);
	}
	return ((void *)0);
}
