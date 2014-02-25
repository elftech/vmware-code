
#ifndef SCULL_P_NR_DEVS
#define SCULL_P_NR_DEVS 2   /* scullpipe0 through scullpipe1 */
#endif

#ifndef SCULL_P_BUFFER
#define SCULL_P_BUFFER 4000
#endif

#include <linux/poll.h> /*for poll_table structure*/

//our structures
struct scull_pipe {
        wait_queue_head_t inq, outq;       /* read and write queues */
        char *buffer, *end;                /* begin of buf, end of buf */
        int buffersize;                    /* used in pointer arithmetic */
        char *rp, *wp;                     /* where to read, where to write */
        int nreaders, nwriters;            /* number of openings for r/w */
        struct fasync_struct *async_queue; /* asynchronous readers */
        struct semaphore sem;              /* mutual exclusion semaphore */
        struct cdev cdev;                  /* Char device structure */
};

//we need to copy the ioctl defination of sscull to sscullp(how to??)
extern int          scull_ioctl(struct inode *inode, struct file *filp,
                                unsigned int cmd, unsigned long arg);

//Initialing and clearup functions for sscullp
void scull_p_cleanup(void);
int scull_p_init(dev_t firstdev);

