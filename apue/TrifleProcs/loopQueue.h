#include <unistd.h>

#define MAXQSIZE 100
#define TYPE int 
#define OK 0

typedef struct{
    TYPE *base;
    int front;
    int rear;
}lpQue;

int que_init(lpQue *qp)
{
    qp->base = (TYPE *)malloc(MAXQSIZE * sizeof(TYPE));
    if(!qp->base)
	exit(!OK);
    qp->front=qp->rear=0;
}

int que_in(lpQue *qp,TYPE e)
{
    if((qp->rear+1)%MAXQSIZE == qp->front){
	printf("No more spaces!\n");
	return !OK;
    }
    qp->base[qp->rear]=e;
    if(++qp->rear>=MAXQSIZE)
	qp->rear=0;
    return OK;
}

int que_out(lpQue *qp)
{
    if(qp->front==qp->rear){
	printf("The queue is empty\n");
	return !OK;
    }
    if(++qp->front>=MAXQSIZE)
	qp->front=0;
    return OK;
}
