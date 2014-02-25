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

#include <asm/system.h>
#include <asm/uaccess.h>

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0
#endif

#include "sscull.h" 
#include "sscullp.h"
#include "access.h"

extern struct scull_dev *scull_devices;	/* allocated in scull_init_module */

int scull_major =   SCULL_MAJOR;
int scull_minor =   0;

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPL");

extern int scull_nr_devs;	/* number of bare scull devices */
extern int scull_quantum;
extern int scull_qset;

extern struct file_operations scull_fops;

static void scull_setup_cdev(struct scull_dev *dev, int index);

void scull_cleanup_module(void)
{
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	/* Get rid of our char dev entries */
	if (scull_devices) {
		for (i = 0; i < scull_nr_devs; i++) {
			scull_trim(scull_devices + i);
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}


	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, scull_nr_devs);

	/* and call the cleanup functions for friend devices */
	scull_p_cleanup();
	scull_access_cleanup();

}

int scull_init_module(void)
{
	int result, i;
	dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "ssculls");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,
				"ssculls");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "ssculls: can't get major %d\n", scull_major);
		return result;
	}

        /* 
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

        /* Initialize each device. */
	for (i = 0; i < scull_nr_devs; i++) {
		scull_devices[i].quantum = scull_quantum;
		scull_devices[i].qset = scull_qset;
		init_MUTEX(&scull_devices[i].sem);
		scull_setup_cdev(&scull_devices[i], i);
	}

        /* At this point call the init function for any friend device */
	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);
	dev += scull_p_init(dev);
	dev += scull_access_init(dev);

	return 0; /* succeed */

  fail:
	scull_cleanup_module();
	return result;
}  

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);
    
	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
