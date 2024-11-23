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


long integer_part(long lookup_value){
	return lookup_value / 1000;
}

long decimal_part(long lookup_value){
	return lookup_value % 1000;
}

char *convert(long lookup_value){
	static char buf[10];
	long int_part = integer_part(lookup_value);
	long dec_part = decimal_part(lookup_value);
	sprintf(buf, "%ld.%ld", int_part, dec_part);
	return buf;
}

/*
 * Updates the cached state of a character device
 * based on sensor data. Must be called with the
 * character device state lock held.
 */
static int lunix_chrdev_state_update(struct lunix_chrdev_state_struct *state)
{
	struct lunix_sensor_struct __attribute__((unused)) *sensor;
	WARN_ON ( !(sensor = state->sensor));
	
	debug("entering lunix_chrdev_state_update ...\n");

	uint32_t msr_data;
	uint32_t msr_timestamp;
	
	/*
	 * Any new data available?
	 */
	/* ? */
	// check if the state needs to be updated
	if(lunix_chrdev_state_needs_refresh(state) == 0){
		debug("leaving (no new data)\n");
		return -EAGAIN;
	}
	
	/*
	 * Grab the raw data quickly, hold the
	 * spinlock for as little as possible.
	 */
	/* ? */
	/* Why use spinlocks? See LDD3, p. 119 */
	// need to update the state
	spin_lock_irq(&sensor->lock);
	msr_data = sensor->msr_data[state->type]->values[0];	// grab from sensor -> msr_data -> ... -> values[] 
	msr_timestamp = sensor->msr_data[state->type]->last_update;
	spin_unlock_irq(&sensor->lock);

	/*
	 * Now we can take our time to format them,
	 * holding only the private state semaphore
	 */
	/* ? */
	// update is called with the semaphore lock held (we don't need to lock again)

	// update the state
	// use the lookup tables to convert the raw data to textual info
	// update the below fields for chrdev state struct:
	// buf_lim, buf_timestamp, buf_data
	switch(state->type) {
		case BATT:
			state->buf_lim = snprintf(state->buf_data, LUNIX_CHRDEV_BUFSZ, "Battery: %s\n", convert(lookup_voltage[msr_data]));
			break;
		case TEMP:
			state->buf_lim = snprintf(state->buf_data, LUNIX_CHRDEV_BUFSZ, "Temp: %s\n", convert(lookup_temperature[msr_data]));
			break;
		case LIGHT:
			state->buf_lim = snprintf(state->buf_data, LUNIX_CHRDEV_BUFSZ, "Light: %s\n", convert(lookup_light[msr_data]));
			break;
		default:
			printk(KERN_ERR "This measurement isn't supported (Only 0,1,2 are supported)\n");
			up(&state->lock);
			return -EINVAL;
	}
	state->buf_timestamp = msr_timestamp;

	debug("leaving (update ok)\n");
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
	int count;

	state = filp->private_data;
	WARN_ON(!state);

	sensor = state->sensor;
	WARN_ON(!sensor);

	debug("entering read\n");

	/* Lock? */
	if (down_interruptible(&state->lock))
		return -ERESTARTSYS;
	/*
	 * If the cached character device state needs to be
	 * updated by actual sensor data (i.e. we need to report
	 * on a "fresh" measurement, do so
	 */
	if (*f_pos == 0) {
		while (true) {
			int update_ret = lunix_chrdev_state_update(state);
			if(update_ret == -EINVAL) {
				debug("(invalid argument)\n");
				ret = -EINVAL;
				goto unlock_and_out;
			}
			if(update_ret == 0) break;	// update done (exit while loop)

			if(update_ret != -EAGAIN) {
				debug("(unexpected return value in update)\n");
				ret = update_ret;
				goto unlock_and_out;
			}
			// if update_ret == -EAGAIN
			/* ? */
			/* The process needs to sleep */
			/* See LDD3, page 153 for a hint */
			// wait for the sensor to update the state
			up(&state->lock);	// up for semaphore lock
			debug("putting process to sleep until new \n");
			if(wait_event_interruptible(sensor->wq, lunix_chrdev_state_needs_refresh(state)))
				return -ERESTARTSYS;

			if (down_interruptible(&state->lock))
				return -ERESTARTSYS;
		}
		
		// here the state has been updated and semaphore lock is down
	}

	/* Determine the number of cached bytes to copy to userspace */
	/* ? */
	count = min(cnt, (size_t) state->buf_lim - *f_pos);
	if(copy_to_user(usrbuf, state->buf_data + *f_pos, count)){	// copy_to_user returns 0 on success
		debug("leaving (copy_to_user failed)\n");
		ret = -EFAULT;
		goto unlock_and_out;
	}
	
	*f_pos += count;	// update the file position

	/* Return the read number of bytes */
	ret = count;

	/* End of file */
	/* ? */
	/* Auto-rewind on EOF mode? */
	/* ? */
	if (*f_pos == state->buf_lim) {
		*f_pos = 0;	// reset the file position to start
	}

	/*
	 * The next two lines  are just meant to suppress a compiler warning
	 * for the "unused" out: label, and for the uninitialized "ret" value.
	 * It's true, this helpcode is a stub, and doesn't use them properly.
	 * Remove them when you've started working on this code.
	 */
	//ret = -ENODEV;
	//goto out;
unlock_and_out:
	up(&state->lock);	/* Unlock? */
	debug("leaving read, with ret = %d\n", (int)ret);
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
	
	debug("completed successfully \nAuthors: Filothei and Nikolaos\n");
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
