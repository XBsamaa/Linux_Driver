#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>

static int led_cnt = 0;

extern void mydevice_ledcreate(int minor, const char* name);
extern void mydevice_leddestory(int minor);

static int myimx6ull_100ask_led_probe(struct platform_device *pdev)
{
	int i = 0;
	struct resource* res;

	while(1){
		//获取资源文件
		res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
		if(!res)
			break;
		//向内核注册设备
		mydevice_ledcreate(i, res->name);
		++i;
		++led_cnt;
	}
	
	//映射地址空间
	printk("%s %s %d",__FILE__,__FUNCTION__,__LINE__);
	return 0;
}

static int myimx6ull_100ask_led_remove(struct platform_device *pdev)
{	
	int i = 0;
	for(; i < led_cnt; ++i)
	{
		mydevice_leddestory(i);
	};
	return 0;
}


static struct platform_driver myimx6ull_100ask_led_drv = 
{
	.driver = {
		.name 	=	"myimx6ull_myled",
	},
	.probe		=	myimx6ull_100ask_led_probe,
	.remove	=	myimx6ull_100ask_led_remove,
};

static int __init myimx6ull_100ask_led_init(void)
{
	int err;
	
	err = platform_driver_register(&myimx6ull_100ask_led_drv);
	printk("%s %s %d err = %d.\n",__FILE__,__FUNCTION__,__LINE__,err);

	return 0;
}

static void __exit myimx6ull_100ask_led_exit(void)
{
	platform_driver_unregister(&myimx6ull_100ask_led_drv);
}

module_init(myimx6ull_100ask_led_init);
module_exit(myimx6ull_100ask_led_exit);

MODULE_LICENSE("GPL");
