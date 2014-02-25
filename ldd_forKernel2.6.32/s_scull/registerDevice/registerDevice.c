#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>

int device_major=0,device_minor=0;
int b_dev_num=3,c_dev_num=1;
struct cdev b_dev,c_dev;

static int registerDevice_init()
{
     int err,result;
     dev_t dev;

     if (device_major) {
        //To unregister what has been registered improperly
	dev = MKDEV(device_major, device_minor);

	unregister_chrdev_region(dev, 1);
        return 0;
     } else {
	result = alloc_chrdev_region(&dev, device_minor, 3,"BBBBB");
        device_major = MAJOR(dev);
        device_minor = MINOR(dev);

        if (result < 0) 
		printk(KERN_WARNING "ssculls: can't get major %d\n", device_major);
        printk(KERN_ALERT "*******************************************************************************************************************\n");

        dev = MKDEV(device_major, device_minor + b_dev_num);
        result = register_chrdev_region(dev, 1,"CCCCC");
        if (result < 0) 
		printk(KERN_WARNING "ssculls: can't get major %d\n", device_major);

     }

     err = cdev_add (&b_dev, dev, 1);
     err = cdev_add (&c_dev, dev, 1);

     return 0;
}

static int registerDevice_exit()
{
    dev_t devno = MKDEV(device_major, device_minor);
    cdev_del(&b_dev);
    cdev_del(&c_dev);
    unregister_chrdev_region(devno, 3);

    devno = MKDEV(device_major, device_minor+b_dev_num);
    unregister_chrdev_region(devno, 1);
    return 0;
}

module_init(registerDevice_init);
module_exit(registerDevice_exit);
