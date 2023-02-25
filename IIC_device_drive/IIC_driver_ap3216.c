#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

//chrdev设备号,类
static dev_t g_devNum;
static struct class *ap3216Class;
static struct cdev *ap3216Chrdev;
static struct i2c_client *ap3216Client;

int ap3216Fops_open(struct inode *node, struct file *file)
{
	int err;
	//init ap3216c
	err = i2c_smbus_write_byte_data(ap3216Client, 0x00, 0x04);
	//set System Mode：0x00 = 011: ALS and PS+IR functions active
	err = i2c_smbus_write_byte_data(ap3216Client, 0x00, 0x03);
	return 0;
}

ssize_t ap3216Fops_read(struct file *file, char __user *buf, size_t size, loff_t * loff)
{
	char data[6];
	int err, temp;
	
	mdelay(300);
	
	//IR 2 BYTE
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0A);
	data[0] = temp&0xff;
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0B);
	data[1] = temp&0xff;
	
	//ALS 2 BYTE
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0C);
	data[2] = temp&0xff;
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0D);
	data[3] = temp&0xff;
	
	//PS 2 BYTE
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0E);
	data[4] = temp&0xff;
	temp = i2c_smbus_read_byte_data(ap3216Client, 0x0F);
	data[5] = temp&0xff;
	
	
	err = copy_to_user(buf, data, 6);
	if(err < 0){
		printk("FUN: copy_to_user err.\n");
		return err;
	}
	
	return size;
};


static struct file_operations ap3216Fops = 
{
	.owner = THIS_MODULE,
	.open = ap3216Fops_open,
	.read = ap3216Fops_read,
};

static struct i2c_device_id ap3216_ids[] =
{
	{"ap3216",},
	{/*end of list*/},
};

static struct of_device_id ap3216_of_match_ids[] =
{
	{.compatible = "atmel,ap3216",},
	{/*end of list*/},
};

static int ap3216_probe(struct i2c_client *ap3216_Client, const struct i2c_device_id *ap3216DeviceId)
{
	int err;

	ap3216Client = ap3216_Client;
	printk("The I2C client %s address = 0x%x.\n", ap3216Client->name, ap3216Client->addr);
	
	//向内核申请chrdev,并添加设备节点
	err = alloc_chrdev_region(&g_devNum, 0, 1, "ap3216Chrdev");
	if(err < 0){
		printk("FUN: alloc_chrdev_region err.\n");
		return err;
	}
	ap3216Chrdev = cdev_alloc(); 
	cdev_init(ap3216Chrdev, &ap3216Fops);
	cdev_add(ap3216Chrdev, g_devNum, 1);

	ap3216Class = class_create(THIS_MODULE, "ap3216Class");
	device_create(ap3216Class, NULL, g_devNum, NULL, "ap3216Chrdev");
	
	return 0;
};
	
static int ap3216_remove(struct i2c_client *ap3216_Client)
{
	//删除设备节点，并释放内存
	device_destroy(ap3216Class, g_devNum);
	class_destroy(ap3216Class);
	cdev_del(ap3216Chrdev);
	unregister_chrdev_region(g_devNum, 1);
	return 0;
};

static struct i2c_driver ap3216_i2c_driver =
{
	.driver = {
			.name = "ap3216_driver",
			.of_match_table = ap3216_of_match_ids,
		},
	.probe	= ap3216_probe,
	.remove	= ap3216_remove,
	.id_table	= ap3216_ids,
};


static int __init ap3216_driver_init(void)
{
	//向内核i2c_driver结构体
	i2c_register_driver(THIS_MODULE, &ap3216_i2c_driver);
	return 0;
};

static void __exit ap3216_driver_exit(void)
{
	//删除i2c_driver结构体
	i2c_del_driver(&ap3216_i2c_driver);
};

module_init(ap3216_driver_init);
module_exit(ap3216_driver_exit);
MODULE_LICENSE("GPL");
