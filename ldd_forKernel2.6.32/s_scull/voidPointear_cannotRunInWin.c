#include <stdlib.h>

void main()
{
    void **p;
    p=malloc(10*sizeof(char *));
    p[0]=malloc(10);
}
