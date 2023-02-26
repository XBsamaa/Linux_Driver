#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>


static struct i2c_adapter *i2c_adapter_virtual;


static int i2c_algo_xfer_virtual(struct i2c_adapter *adap, struct i2c_msg *msgs,int num)
{
	return 0;
};

static u32 i2c_algo_func_virtual(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_NOSTART | I2C_FUNC_SMBUS_EMUL |
	       I2C_FUNC_SMBUS_READ_BLOCK_DATA |
	       I2C_FUNC_SMBUS_BLOCK_PROC_CALL |
	       I2C_FUNC_PROTOCOL_MANGLING;
};



static const struct i2c_algorithm i2c_adapter_algo_virtual = {
	.master_xfer = i2c_algo_xfer_virtual,
	.functionality = i2c_algo_func_virtual,
};

static struct of_device_id i2c_bus_virtual_ids[] = {
	{ .compatible = "i2c_bus_virtual", },
	{ /* sentinel */ }
};

static int i2c_bus_driver_virtual_probe(struct platform_device *pdev)
{
	int err;
	//to alloc、set、add i2c adapter
	i2c_adapter_virtual = kzalloc(sizeof(*i2c_adapter_virtual), GFP_KERNEL);
	i2c_adapter_virtual->owner = THIS_MODULE;
	i2c_adapter_virtual->class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	i2c_adapter_virtual->nr = -1;
	//i2c_adapter_virtual->algo = &i2c_adapter_algo_virtual;

	snprintf(i2c_adapter_virtual->name, sizeof(i2c_adapter_virtual->name), "i2c_bus_virtual");

	err = i2c_add_adapter(i2c_adapter_virtual);
	if(err < 0){
		printk("FUNC: i2c_add_adapter err.\n");
		return err;
	}
	
	
	return 0;
};
static int i2c_bus_driver_virtual_remove(struct platform_device *pdev)
{
	return 0;
};



static struct platform_driver i2c_bus_driver_virtual = {
	.driver = {
		.name = "i2c_bus_virtual",
		.of_match_table = of_match_ptr(i2c_bus_virtual_ids),
	},
	.probe		= i2c_bus_driver_virtual_probe,
	.remove 	= i2c_bus_driver_virtual_remove,
};

static int __init i2c_bus_driver_virtual_init(void)
{
	int err;
	
	err = platform_driver_register(&i2c_bus_driver_virtual);
	if(err < 0){
		printk("FUNC: platform_driver_register err.\n");
		return err;
	}
	
	return 0;
};

static void __exit i2c_bus_driver_virtual_exit(void)
{
	platform_driver_unregister(&i2c_bus_driver_virtual);
};

module_init(i2c_bus_driver_virtual_init);
module_exit(i2c_bus_driver_virtual_exit);
MODULE_LICENSE("GPL");
