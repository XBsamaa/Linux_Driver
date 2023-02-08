#include <linux/module.h>
#include <linux/io.h>
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
#include <linux/ioport.h>

#include "led_opr.h"


static int led_cnt = 0;

static int *GPIO1_GDIR;
static int *GPIO1_DR;
static int *GPIO_LED;

//记录资源文件，提供给操作函数
static struct resource *led_rsc;

extern void get_myled_opr(struct myled_opr * myled_opr);

extern void mydevice_ledcreate(int minor, const char* name);

extern void mydevice_leddestory(int minor);

static void board_myimx6ull_led_opr_init(int minor)
{
	
	//将物理地址与虚拟地址联系起来
	GPIO1_GDIR = ioremap(led_rsc[0].end, 4);
	GPIO1_DR = ioremap(led_rsc[1].end, 4);
	GPIO_LED = ioremap(led_rsc[minor].start, 4);
	
	//将pad设置为GPIO模式
	*GPIO_LED &= ~(0xf);
	*GPIO_LED |= (0x5);

};

static char board_myimx6ull_led_opr_read(int minor)
{
	char status;
	//将GPIO口设置为读模式,并读取数值
	switch(minor){
		case 0:
			*GPIO1_GDIR &= ~(1<<3);
			status =  (*GPIO1_DR) & (1<<3);
			break;
		case 1:
			*GPIO1_GDIR &= ~(1<<5);
			status =  (*GPIO1_DR) & (1<<5);
			break;
		case 2:
			*GPIO1_GDIR &= ~(1<<6);
			status =  (*GPIO1_DR) & (1<<6);
			break;
		default:
			break;	
	}

	return status;
};

static void board_myimx6ull_led_opr_write(int minor, char status)
{

};


static struct myled_opr board_myimx6ull_led_opr = 
{
	.myled_init		=	board_myimx6ull_led_opr_init,
	.myled_read		=	board_myimx6ull_led_opr_read,
	.myled_write	=	board_myimx6ull_led_opr_write,	
};

static int myimx6ull_100ask_led_probe(struct platform_device *pdev)
{
	struct resource* res;

	while(1){
		//获取资源文件
		res = platform_get_resource(pdev, IORESOURCE_IRQ, led_cnt);
		
		if(!res)
			break;
		
		//向内核注册设备
		mydevice_ledcreate(led_cnt, res->name);

		//记录资源文件
		led_rsc[led_cnt] = *res;

		//记录led数量
		led_cnt++;
		}
	
	printk("%s %s %d",__FILE__,__FUNCTION__,__LINE__);
	return 0;

	get_myled_opr(&board_myimx6ull_led_opr);
}

static int myimx6ull_100ask_led_remove(struct platform_device *pdev)
{	
	iounmap(GPIO1_GDIR);
	iounmap(GPIO1_DR);
	iounmap(GPIO_LED);
	
	while(led_cnt--)
	{
		//向内核销毁设备
		mydevice_leddestory(led_cnt);
	};
	return 0;
};


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
};

static void __exit myimx6ull_100ask_led_exit(void)
{
	platform_driver_unregister(&myimx6ull_100ask_led_drv);
};

module_init(myimx6ull_100ask_led_init);
module_exit(myimx6ull_100ask_led_exit);

MODULE_LICENSE("GPL");
