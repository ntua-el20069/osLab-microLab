/*
 * lunix-chrdev.c
 *
 * Implementation of character devices
 * for Lunix:TNG
 *
 * Filothei Linardatou
 * Nikolaos Papakonstantopoulos
 */

#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mmzone.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h>

#include "lunix.h"
#include "lunix-chrdev.h"
#include "lunix-lookup.h"

/*
 * Global data
 */
struct cdev lunix_chrdev_cdev;

/*
 * Just a quick [unlocked] check to see if the cached
 * chrdev state needs to be updated from sensor measurements.
 */
/*
 * Declare a prototype so we can define the "unused" attribute and keep
 * the compiler happy. This function is not yet used, because this helpcode
 * is a stub.
 */
static int __attribute__((unused)) lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *);
static int lunix_chrdev_state_needs_refresh(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct *sensor;
	
	WARN_ON ( !(sensor = state->sensor));
	/* ? */
	if(sensor->msr_data[state->type]->last_update > state->buf_timestamp)
		return 1;

	return 0; 
}

/*
 * Updates the cached state of a character device
 * based on sensor data. Must be called with the
 * character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct __attribute__((unused)) *sensor;
	
	debug("leaving\n");

	/*
	 * Grab the raw data quickly, hold the
	 * spinlock for as little as possible.
	 */
	/* ? */
	/* Why use spinlocks? See LDD3, p. 119 */

	/*
	 * Any new data available?
	 */
	/* ? */

	/*
	 * Now we can take our time to format them,
	 * holding only the private state semaphore
	 */

	/* ? */

	debug("leaving\n");
	return 0;
}

/*************************************
 * Implementation of file operations
 * for the Lunix character device
 *************************************/

static int lunix_chrdev_open(struct inode *inode, struct file *filp)
{
	/* Declarations */
	/* ? */
	int ret;
	struct lunix_chrdev_state_struct *state;
    int minor = iminor(inode);

	debug("entering\n");
	ret = -ENODEV;

	// Ensure the file does not open in write only or read and write mode
	if ((filp->f_flags & O_WRONLY) || (filp->f_flags & O_RDWR))
    return -EPERM;

	if ((ret = nonseekable_open(inode, filp)) < 0)
		goto out;

	/*
	 * Associate this open file with the relevant sensor based on
	 * the minor number of the device node [/dev/sensor<NO>-<TYPE>]
	 */
	state = kmalloc(sizeof(struct lunix_chrdev_state_struct), GFP_KERNEL);
    if (!state) {
        printk(KERN_ERR "Failed to allocate memory for character device private state\n");
		ret = -ENOMEM;
        goto out;
    }

	// use the minor number to determine the sensor and the type of chrdev state struct 
	state->type = minor & 0x7;
	state->sensor = &lunix_sensors[minor >> 3];
	// initialize buffer and semaphore
	state->buf_lim = 0;
	state->buf_timestamp = 0;
	sema_init(&state->lock, 1);
	
	/* Allocate a new Lunix character device private state structure */
	/* ? */
	filp->private_data = state;
	debug("character device state initialized successfully\n");

	switch (state->type) {
		case BATT: 
			debug("/dev/sensor%d-batt opened\n", minor >> 3);
			break;
		case TEMP: 
			debug("/dev/sensor%d-temp opened\n", minor >> 3);
			break;
		case LIGHT: 
			debug("/dev/sensor%d-light opened\n", minor >> 3);
			break;
		default:
			/* Only battery, temperature and light measurements are supported */
			printk(KERN_ERR "This measurement isn't supported (Only 0,1,2 are supported)\n");
			goto out;
	}

    ret = 0;

out:
	debug("leaving, with ret = %d\n", ret);
	return ret;
}

static int lunix_chrdev_release(struct inode *inode, struct file *filp)
{
	WARN_ON(!filp->private_data);
	kfree(filp->private_data);
	debug("character device state memory released\n");
	return 0;
}

static long lunix_chrdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return -EINVAL;
}

static ssize_t lunix_chrdev_read(struct file *filp, char __user *usrbuf, size_t cnt, loff_t *f_pos)
{
	ssize_t ret;

	struct lunix_sensor_struct *sensor;
	struct lunix_chrdev_state_struct *state;

	state = filp->private_data;
	WARN_ON(!state);

	sensor = state->sensor;
	WARN_ON(!sensor);

	/* Lock? */
	/*
	 * If the cached character device state needs to be
	 * updated by actual sensor data (i.e. we need to report
	 * on a "fresh" measurement, do so
	 */
	if (*f_pos == 0) {
		while (lunix_chrdev_state_update(state) == -EAGAIN) {
			/* ? */
			/* The process needs to sleep */
			/* See LDD3, page 153 for a hint */
		}
	}

	/* End of file */
	/* ? */
	
	/* Determine the number of cached bytes to copy to userspace */
	/* ? */

	/* Auto-rewind on EOF mode? */
	/* ? */

	/*
	 * The next two lines  are just meant to suppress a compiler warning
	 * for the "unused" out: label, and for the uninitialized "ret" value.
	 * It's true, this helpcode is a stub, and doesn't use them properly.
	 * Remove them when you've started working on this code.
	 */
	ret = -ENODEV;
	goto out;
out:
	/* Unlock? */
	return ret;
}

static int lunix_chrdev_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return -EINVAL;
}

static struct file_operations lunix_chrdev_fops = 
{
	.owner          = THIS_MODULE,
	.open           = lunix_chrdev_open,
	.release        = lunix_chrdev_release,
	.read           = lunix_chrdev_read,
	.unlocked_ioctl = lunix_chrdev_ioctl,
	.mmap           = lunix_chrdev_mmap
};

int lunix_chrdev_init(void)
{
	/*
	 * Register the character device with the kernel, asking for
	 * a range of minor numbers (number of sensors * 8 measurements / sensor)
	 * beginning with LINUX_CHRDEV_MAJOR:0
	 */
	int ret;
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;

	debug("initializing character device\n");
	cdev_init(&lunix_chrdev_cdev, &lunix_chrdev_fops);
	lunix_chrdev_cdev.owner = THIS_MODULE;
	
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	/* ? */
	/* register_chrdev_region? */
	ret = register_chrdev_region(dev_no, lunix_minor_cnt, "lunix_chrdev");
	if (ret < 0) {
		debug("failed to register region, ret = %d\n", ret);
		goto out;
	}
	/* ? */
	/* cdev_add? */
	ret = cdev_add(&lunix_chrdev_cdev, dev_no, lunix_minor_cnt);
	if (ret < 0) {
		debug("failed to add character device\n");
		goto out_with_chrdev_region;
	}
	
	debug("completed successfully\n");
	return 0;

out_with_chrdev_region:
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
out:
	return ret;
}

void lunix_chrdev_destroy(void)
{
	dev_t dev_no;
	unsigned int lunix_minor_cnt = lunix_sensor_cnt << 3;

	debug("entering\n");
	dev_no = MKDEV(LUNIX_CHRDEV_MAJOR, 0);
	cdev_del(&lunix_chrdev_cdev);
	unregister_chrdev_region(dev_no, lunix_minor_cnt);
	debug("leaving\n");
}
