/*
* 通用IIC驱动代码，将example替换成设备名称即可。
* XBsama,XBsama_@hotmail.com
*/

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>

//chrdev,设备号,类,i2c设备
static dev_t g_devNum;
static struct class *exampleClass;
static struct cdev *exampleChrdev;
static struct i2c_client *exampleClient;

int exampleFops_open(struct inode *node, struct file *file)
{
	//对i2c设备进行初始化
	return 0;
}

ssize_t exampleFops_read(struct file *file, char __user *buf, size_t size, loff_t * loff)
{
	//对i2c设备进行读操作
	return size;
};


static struct file_operations exampleFops = 
{
	.owner = THIS_MODULE,
	.open = exampleFops_open,
	.read = exampleFops_read,
};

static struct i2c_device_id example_ids[] =
{
	{"example",},
	{/*end of list*/},
};

static struct of_device_id example_of_match_ids[] =
{
	{.compatible = "company,example",},
	{/*end of list*/},
};

static int example_probe(struct i2c_client *i2c_client, const struct i2c_device_id *exampleDeviceId)
{
	int err;
	exampleClient = i2c_client;
	
	//向内核申请chrdev,并添加设备节点
	err = alloc_chrdev_region(&g_devNum, 0, 1, "exampleChrdev");
	if(err < 0){
		printk("FUN: alloc_chrdev_region err.\n");
		return err;
	}
	exampleChrdev = cdev_alloc();
	cdev_init(exampleChrdev, &exampleFops);
	err = cdev_add(exampleChrdev, g_devNum, 1);
	if(err <0){
		printk("FUN: cdev_add err.\n");
		return err;
	}
	exampleClass = class_create(THIS_MODULE, "exampleClass");
	device_create(exampleClass, NULL, g_devNum, NULL, "exampleChrdev");
	
	return 0;
};
	
static int example_remove(struct i2c_client *exampleClient)
{
	//删除设备节点，并释放内存
	device_destroy(exampleClass, g_devNum);
	class_destroy(exampleClass);
	cdev_del(exampleChrdev);
	unregister_chrdev_region(g_devNum, 1);
	return 0;
};

static struct i2c_driver example_i2c_driver =
{
	.driver = {
			.name = "example_driver",
			.of_match_table = example_of_match_ids,
		},
	.probe	= example_probe,
	.remove	= example_remove,
	.id_table	= example_ids,
};


static int __init example_driver_init(void)
{
	//向内核i2c_driver结构体
	i2c_register_driver(THIS_MODULE, &example_i2c_driver);
	return 0;
};

static void __exit example_driver_exit(void)
{
	//删除i2c_driver结构体
	i2c_del_driver(&example_i2c_driver);
};

module_init(example_driver_init);
module_exit(example_driver_exit);
MODULE_LICENSE("GPL");
