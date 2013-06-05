/*===============================================================

 * File Name : readerWriterProblem.c
 * Creation Date : 2013-04-11
 * Last Modified : 2013年04月11日 星期四 22时55分04秒
 * Create By : Binyuan
 * Email : elf.ggle@gmail.com
 * Purpose :

================================================================*/

#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define READER_NUM 5
void *reader_handler(void *ptr);
void *writer_handler(void *ptr);

sem_t write_mutex;
sem_t mutex;
int read_count;

void main()
{
	sem_init(&write_mutex,0,1);
	sem_init(&mutex,0,1);
	read_count=0;
	
	int err,i;
	pthread_t reader[READER_NUM],writer;
	int args[READER_NUM];
	for(i=0;i<READER_NUM;++i){
		args[i]=i;
		err=pthread_create(&reader[i],NULL,reader_handler,(void*)&args[i]);
		if(err!=0)
			err_quit("can't create thread: %s\n",strerror(err));
	}
	err=pthread_create(&writer,NULL,writer_handler,NULL);
	if(err!=0)
		err_quit("can't create thread: %s\n",strerror(err));

	sleep(10);

}

void *reader_handler(void *ptr)
{
	while(1){
		if(read_count==0){
			sem_wait(&write_mutex);
		}
		sem_wait(&mutex);
		++read_count;
		sem_post(&mutex);

		printf("There are %d readers reading\n",read_count);
		sleep(1);

		sem_wait(&mutex);
		--read_count;
		sem_post(&mutex);

		if(read_count==0){
			sem_post(&write_mutex);
		}
		sleep(1);
	}
	return ((void *)0);
}

void *writer_handler(void *ptr)
{
	while(1){
		if(read_count==0){
			sem_wait(&write_mutex);
			printf("Writer writes\n");
			sem_post(&write_mutex);
		}
		else
			printf("Writer failed\n");
		sleep(1);
	}
	return ((void *)0);
}
