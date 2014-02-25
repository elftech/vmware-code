#include "stdio.h"
#include "stdlib.h"

int resultW(int a1,int b1,int a2,int b2)
{
    int j,t;
   // printf("%d %d %d %d\n",a1,b1,a2,b2);
    if(a2>=b1)
	return 0;
    else if(b2<=b1){
	j=b2-a2;
	t=b1-a1;
    }
    else {
	j=b1-a2;
	t=b2-a1;
    }
  //  printf("%d %d\n",t,j);
    return t*j;
}

void main()
{
    int num,i,*pa,*pb,j,tmp,max=0;
    scanf("%d",&num);
    pa=(int *)malloc(num*sizeof(int));
    pb=(int *)malloc(num*sizeof(int));
    for(i=0;i<num;++i)
	scanf("%d %d",pa+i,pb+i);
    for(i=0;i<num-1;++i)
	for(j=i+1;j<num;++j){
	    if(pa[j]>pa[i])
		tmp=resultW(pa[i],pb[i],pa[j],pb[j]);
	    else
		tmp=resultW(pa[j],pb[j],pa[i],pb[i]);
	    if(tmp>max)
		max=tmp;
	}

    printf("%d\n",max);
}
