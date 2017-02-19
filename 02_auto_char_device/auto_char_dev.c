/*  
 *  auto_char_dev.c - The simplest kernel module.
 */

#include <linux/init.h>		// Needed by init/exit macros
#include <linux/module.h>	// Needed by all modules
#include <linux/kernel.h>	// Needed for KERN_INFO
#include <linux/fs.h>		// Needed for file operations
#include <linux/cdev.h>		// Device Registration
#include <linux/device.h>		// Udev class creation

MODULE_AUTHOR("Allen Mamaril");      // The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver");  // The description -- see modinfo
MODULE_VERSION("0.1");              // The version of the module
MODULE_LICENSE("GPL");

#define MOD_NAME "chdx"

static dev_t first;
static unsigned int count = 1;
static struct device *chdx_dev;
static struct cdev *chdx_cdev;				// char device structure
static struct class *chdx_cl;
static const char device_name[] = "Simplest-driver";

static struct file_operations simplest_driver_fops =
{
	.owner	= THIS_MODULE,
};

int __init km_init(void)
{
	int ret = 0;

	/* Dynamic Allocation of major number */
	printk(KERN_NOTICE "%s: Registering Device\n", __FUNCTION__);
	ret = alloc_chrdev_region(&first, 0, count, MOD_NAME);

	if(ret < 0)
	{
		dev_err(chdx_dev, "Character device registration error!\n");
		return ret;
	}

	/* Device registration */
	chdx_cdev = cdev_alloc();

	if(ret != 0)
	{
		dev_err(chdx_dev, "cdev_alloc() failed\n");
		goto unreg1;
	}

	cdev_init(chdx_cdev, &simplest_driver_fops);
	
	ret = cdev_add(chdx_cdev, first, count);

	if(ret != 0)
	{
		dev_err(chdx_dev, "cdev_add() failed\n");
		goto unreg2;
	}

	chdx_cl = class_create(THIS_MODULE, MOD_NAME);

	if(ret != 0)
	{
		goto unreg2;
	}

	chdx_dev = device_create(chdx_cl, NULL, first, NULL, "%s", MOD_NAME);
	
	if(chdx_dev == NULL)
	{
		goto unreg3;
	}

	unreg3: device_destroy(chdx_cl, first);
	unreg2: cdev_del(chdx_cdev);
	unreg1: unregister_chrdev_region(first, count);

	return ret;
}

void __exit km_exit(void)
{
	printk(KERN_INFO "%s: Unregistering Character Device\n", __FUNCTION__);

	device_destroy(chdx_cl, first);
	class_destroy(chdx_cl);
	cdev_del(chdx_cdev);
	unregister_chrdev_region(first, count);
	dev_info(chdx_dev, "chdx unloaded");

	printk(KERN_INFO "%s: Goodbye Kernal Land!\n", __FUNCTION__);
}

module_init(km_init);
module_exit(km_exit);
