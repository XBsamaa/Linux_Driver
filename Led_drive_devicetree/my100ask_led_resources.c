#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>


static struct resource myledsrc[] = 
{
	{
		.start	=	0,
		.flags	=	IORESOURCE_IRQ,
		.name	=	"myled2[1_3]",
	},
	{
		.start	=	0,
		.flags	=	IORESOURCE_IRQ,
		.name	=	"myled3[1_5]",
	},
	{
		.start	=	0,
		.flags	=	IORESOURCE_IRQ,
		.name	=	"myled4[1_6]",
	},
	{},
};

void myimx6ull_100ask_led_dev_remove(struct device *dev){
}


static struct platform_device myimx6ull_100ask_led_dev = 
{	
	.name			=	"myimx6ull_myled",
	.num_resources 	=	ARRAY_SIZE(myledsrc),
	.resource 		=	myledsrc,
	.dev			=	{
		.release	=	myimx6ull_100ask_led_dev_remove,
	},
};

static int __init myimx6ull_100ask_led_dev_init(void)
{
	int err;
	
	err = platform_device_register(&myimx6ull_100ask_led_dev);

	return 0;
}

static void __exit myimx6ull_100ask_led_dev_exit(void)
{
	platform_device_unregister(&myimx6ull_100ask_led_dev);
}

module_init(myimx6ull_100ask_led_dev_init);
module_exit(myimx6ull_100ask_led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XBsama <XBsama_@hotmail.com>");
