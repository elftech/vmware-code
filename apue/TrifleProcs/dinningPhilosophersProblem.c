/*===============================================================

 * File Name : dinningPhilosophersProblem.c
 * Creation Date : 2013-04-11
 * Last Modified : 2013年04月11日 星期四 21时34分18秒
 * Create By : Binyuan
 * Email : elf.ggle@gmail.com
 * Purpose :

================================================================*/

#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define PHILO_NUM 5
void *philosopher(void *arg);

sem_t sema[PHILO_NUM];
void main()
{
	int i,err;
	int args[PHILO_NUM];
	for(i=0;i<PHILO_NUM;++i){
		sem_init(sema+i,0,1);
		args[i]=i;
	}
	pthread_t philosophers[PHILO_NUM];
	for(i=0;i<PHILO_NUM;++i){
		err=pthread_create(philosophers+i,NULL,philosopher,(void*)&args[i]);
		if(err!=0)
			err_quit("can't create thread: %s\n",strerror(err));
	}
	sleep(5);
}

void *philosopher(void *arg)
{
	int num;
	num=*((int *)arg);
	//printf("Philosopher %d eats\n",num);
	while(1){ 
		sem_wait(&sema[num]);
		sem_wait(&sema[(num+1)%PHILO_NUM]);
		printf("Philosopher %d eats\n",num);
		sem_post(&sema[num]);
		sem_post(&(sema[(num+1)%PHILO_NUM]));
		sleep(1);
	}

	return ((void *)0);
}
