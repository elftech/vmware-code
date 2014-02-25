#include "stdio.h"

static int sbull_make_request(char *q,int *bio)
{
    printf("hello,nice to meet you!\n");
    return 0;
}

typedef int (make_request_fn) (char *q,int *bio);

void blk_queue_make_request(int n,make_request_fn *fn)
{
    (*fn)(NULL,&n);
}

void main()
{
    blk_queue_make_request(1,sbull_make_request);
}

