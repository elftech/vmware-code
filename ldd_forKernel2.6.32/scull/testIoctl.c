#include   <stdio.h>
#include   <fcntl.h>
#include   <sys/ioctl.h>
#include   <sys/types.h>
#include   <linux/kdev_t.h>
#include   <stdlib.h>

#include <linux/ioctl.h> 

/* Use 'k' as magic number */
#define SCULL_IOC_MAGIC  'k'
/* Please use a different 8-bit number in your code */

#define SCULL_IOCRESET    _IO(SCULL_IOC_MAGIC, 0)
/*
 * S means "Set" through a ptr,
 * T means "Tell" directly with the argument value
 * G means "Get": reply by setting through a pointer
 * Q means "Query": response is on the return value
 * X means "eXchange": switch G and S atomically
 * H means "sHift": switch T and Q atomically
 */
#define SCULL_IOCSQUANTUM _IOW(SCULL_IOC_MAGIC,  1, int)
#define SCULL_IOCSQSET    _IOW(SCULL_IOC_MAGIC,  2, int)
#define SCULL_IOCTQUANTUM _IO(SCULL_IOC_MAGIC,   3)
#define SCULL_IOCTQSET    _IO(SCULL_IOC_MAGIC,   4)
#define SCULL_IOCGQUANTUM _IOR(SCULL_IOC_MAGIC,  5, int)
#define SCULL_IOCGQSET    _IOR(SCULL_IOC_MAGIC,  6, int)
#define SCULL_IOCQQUANTUM _IO(SCULL_IOC_MAGIC,   7)
#define SCULL_IOCQQSET    _IO(SCULL_IOC_MAGIC,   8)
#define SCULL_IOCXQUANTUM _IOWR(SCULL_IOC_MAGIC, 9, int)
#define SCULL_IOCXQSET    _IOWR(SCULL_IOC_MAGIC,10, int)
#define SCULL_IOCHQUANTUM _IO(SCULL_IOC_MAGIC,  11)
#define SCULL_IOCHQSET    _IO(SCULL_IOC_MAGIC,  12)

/*   device   path   */
char   path[]   =   "/dev/scull0";
char   buf[128];
char   temp[128];

int   main()
{
    int  temp11,temp22;
    int   f   =   open(path,   O_RDWR);
    if   (f   ==   -1){
	printf("device   open   error!\n");
	return   1;
    }
    ioctl(f,SCULL_IOCGQUANTUM,&temp11);
    printf("quantum is %d\n" , temp11);
    printf("set quantum to\n" );
    temp11 = 0;
    scanf("%d",&temp11);
    ioctl(f,SCULL_IOCTQUANTUM,temp11);
    ioctl(f,SCULL_IOCGQUANTUM,&temp22);
    printf("quantum is %d\n" , temp22);



    printf("Input   a   string   to   write   device   \n");
    scanf("%s",   buf);
    write(f,   buf,   4); /*   device   wirte   */

    printf("Read   the   string   from   device...\n");
    read(f,   temp,   128); /*   device   read   */
    printf("%s\n",   buf);

    close(f);
}

