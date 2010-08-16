#include <linux/module.h>  /* needed by all modules */
#include <linux/kernel.h>  /* printk */
#include <linux/fs.h>      /* struct file_operations */
#include <asm/uaccess.h>    /* put_user */

/// -- prototype --
static int __init my_init_module(void);
static void __exit my_exit_mod(void);
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEV_NAME "my_char_dev" /* dev name as it appaers in /proc/devices */
#define BUF_LEN 80             /* max length of msg from dev */

/// -- global var ---
static int major;
static int device_open = 0;  /* is dev open? used to prevent multiple open */
static char msg[BUF_LEN]; /* msg dev gvive when asked */
static char *msg_ptr;

static struct file_operations fops={
	.read = dev_read,
	.write = dev_write,
	.open = dev_open,
	.release = dev_release
};

/// -- function --
static int __init my_init_module(void)
{
	major = register_chrdev(0, DEV_NAME, &fops);
	if (major < 0){
		printk(KERN_ALERT "Registering char failed with %d\n", major);
		return major;
	}

	printk(KERN_INFO "my major is %d\n", major);
	printk(KERN_INFO "mknod /dev/%s c %d 0\n", DEV_NAME,major);
	printk(KERN_INFO "try with various minor number\n");
	printk(KERN_INFO "to read, cat /dev/%s\n", DEV_NAME);
	printk(KERN_INFO "to write, echo 'hi' > /dev/%s\n", DEV_NAME);

	return SUCCESS;
}

static void __exit my_exit_mod(void)
{
	unregister_chrdev(major, DEV_NAME);
}

/* called when process try to open, e.g. cat /dev/xxx */
static int dev_open(struct inode *inode, struct file * filp)
{
	static int counter = 0;

	if (device_open)
			return -EBUSY;

	device_open++;
	sprintf(msg, "I have replied %d of times\n", counter++);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* called when process closes the dev file */
static int dev_release(struct inode *inode, struct file *filp)
{
	device_open--; /// ready for next caller
	/// note: decrement usage counter, else not able to get rid of this module
	module_put(THIS_MODULE);

	return 0;
}

/* read from it */
static ssize_t dev_read(struct file *filp,
						char *buf, /// buf to pass back to user
						size_t len, /// length of buf
						loff_t * offset)
{
	/// number of bytes actually written to buf
	int bytes_read = 0;

	/* if at the end of message, return 0 to means end of file */
	if (*msg_ptr == 0)
		return 0;

	/* fill the buf to be sent back to user */
	while (len && *msg_ptr) {
		/// buf is user data segment, not kernel segment. so * assignment
		/// won't work, need to use put_user to copy from kernel to user
		put_user(*(msg_ptr++), buf++);
		len--;
		bytes_read++;
	}

	/// most read functions return # of bytes put into the buffer
	return bytes_read;
}

/* called when 'echo "hi" > /dev/xxx */
static ssize_t dev_write(struct file *filp, const char * buf,
						size_t len, loff_t * off)
{
	printk(KERN_ALERT "write not supported\n");
	return -EINVAL;
}


module_init(my_init_module);
module_exit(my_exit_mod);
