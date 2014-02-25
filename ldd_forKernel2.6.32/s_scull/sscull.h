#ifndef _SCULL_H_
#define _SCULL_H_

#include <linux/ioctl.h>

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 1
#endif

#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

struct scull_qset{
    void **data;
    struct scull_qset *next;
};

struct scull_dev{
    struct scull_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    struct semaphore sem;
    struct cdev cdev;
};

extern int scull_nr_devs;
extern int scull_quantum;
extern int scull_qset;

int     scull_trim(struct scull_dev *dev);
struct scull_qset * \
        scull_follow(struct scull_dev *dev, int n);

ssize_t scull_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos);
ssize_t scull_write(struct file *filp,const char __user *buf,size_t count,loff_t *f_ops);
loff_t  scull_llseek(struct file *filp,loff_t off, int whence);
int     scull_ioctl(struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg);
int     scull_open(struct inode *inode, struct file *filp);
int     scull_release(struct inode *inode, struct file *filp);


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

#define SCULL_IOC_MAXNR 14

#endif /* _SCULL_H_ */
