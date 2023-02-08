#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/uaccess.h>


#include "led_opr.h"


static int major;
static struct class *led_class;
static struct myled_opr *ledopr;

static int led_opt_open (struct inode * node, struct file * file)
{
	int minor;

	minor = MINOR(node->i_rdev);

	ledopr->myled_init(minor);
	
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	return 0;
};

static ssize_t led_opt_read (struct file * file, char __user * buf, size_t size, loff_t *opps)
{
	char status;
	int	minor;
	int err;
	size = 1;
	
	minor = MINOR(file->f_inode->i_rdev);
	status = ledopr->myled_read(minor);
	err = copy_to_user(buf, (status?"1":"0"), size);
	return 0;
}



void mydevice_ledcreate(int minor, const char* name)
{
	device_create(led_class, NULL, MKDEV(major, minor),NULL, name);
};

void mydevice_leddestory(int minor)
{
	device_destroy(led_class, MKDEV(major, minor));
};


static struct file_operations led_fops = 
	{
			.owner	=	THIS_MODULE,
			.open	=	led_opt_open,
			.read	=	led_opt_read,
			.write	=	NULL,
	};

static int __init led_drv_init(void)
{
	//申请主设备号
	major = register_chrdev(0, "myled", &led_fops);

	//辅助申请设备节点
	led_class = class_create(THIS_MODULE, "myledclass");

	return 0;
};

static void __exit led_drv_exit(void)
{
	class_destroy(led_class);
	unregister_chrdev(major, "myled");
};

void get_myled_opr(struct myled_opr *myled_opr){
	ledopr = myled_opr;
};

EXPORT_SYMBOL(mydevice_ledcreate);
EXPORT_SYMBOL(mydevice_leddestory);
EXPORT_SYMBOL(get_myled_opr);



module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XBsama <XBsama_@hotmail.com>");
