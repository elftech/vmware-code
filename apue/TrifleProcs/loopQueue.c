#include "loopQueue.h"

void main()
{
    lpQue Q;
    que_init(&Q);
    int i,j;
    for(i=0;i<110;++i)
	que_in(&Q,i);
    for(i=0;i<110;++i)
	que_out(&Q);
}
