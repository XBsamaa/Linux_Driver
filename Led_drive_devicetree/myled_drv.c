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


static int major;
static struct class *led_class;

void mydevice_ledcreate(int minor, const char* name)
{
	device_create(led_class, NULL, MKDEV(major, minor),NULL, name);
};

void mydevice_leddestory(int minor)
{
	device_destroy(led_class, MKDEV(major, minor));
};


static struct file_operations led_opt = 
	{
			.owner	= THIS_MODULE,
			
	};

static int __init led_drv_init(void)
{
	//申请主设备号
	major = register_chrdev(0, "myled", &led_opt);

	//辅助申请设备节点
	led_class = class_create(THIS_MODULE, "myledclass");

	return 0;
};

static void __exit led_drv_exit(void)
{
	class_destroy(led_class);
	unregister_chrdev(major, "myled");
};

EXPORT_SYMBOL(mydevice_ledcreate);
EXPORT_SYMBOL(mydevice_leddestory);


module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XBsama <XBsama_@hotmail.com>");
