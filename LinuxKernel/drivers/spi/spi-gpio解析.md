# spi-gpio解析

本文对linux内核源码中./drivers/spi/spi-gpio.c进行解析，以深入理解设备驱动模型和SPI驱动。

## 1、spi-gpio驱动初始化函数和出口函数

正常驱动的初始化函数和出口函数，形式如下：

~~~c
static int __init driver_start(void)
{
    ...
}
module_init(driver_start)
    
static void __exit driver_exit(void)
{
    ...
}
module_exit(driver_exit)
/**
 * /linux/module.h
 * #define module_init(x)	__initcall(x);
 * module_init() - driver initialization entry point
 * @x: function to be run at kernel boot time or module insertion
 *
 * module_init() will either be called during do_initcalls() (if
 * builtin) or at module insertion time (if a module).  There can only
 * be one per module.
 */
~~~

但是，spi-gpio驱动采用平台驱动框架编写，而平台驱动框架对驱动初始化和出口函数都进行了一层封装，在调用平台驱动框架编写驱动时，我们只需要定义一个平台驱动结构体（platform_driver）对象，并对重要的成员变量进行初始化即可,如下：

~~~c
static struct platform_driver spi_gpio_driver = {
	.driver = {
		.name	= DRIVER_NAME,								//驱动名
		.of_match_table = of_match_ptr(spi_gpio_dt_ids),    //spi_gpio_dt_ids是of_device_id类型的对象，其字段用于设备和驱动的匹配
	},
	.probe		= spi_gpio_probe,            //驱动与设备匹配时，执行该函数
	.remove		= spi_gpio_remove,			 //驱动注销时，调用该函数
};
module_platform_driver(spi_gpio_driver);     //该宏帮助我们定义了初始化函数和出口函数


/* module_platform_driver() - Helper macro for drivers that don't do
 * anything special in module init/exit.  This eliminates a lot of
 * boilerplate.  Each module may only use this macro once, and
 * calling it replaces module_init() and module_exit()
 */
#define module_platform_driver(__platform_driver) \
        module_driver(__platform_driver, platform_driver_register, \
                        platform_driver_unregister)

/**
 * module_driver() - Helper macro for drivers that don't do anything
 * special in module init/exit. This eliminates a lot of boilerplate.
 * Each module may only use this macro once, and calling it replaces
 * module_init() and module_exit().
 *
 * @__driver: driver name
 * @__register: register function for this driver type
 * @__unregister: unregister function for this driver type
 * @...: Additional arguments to be passed to __register and __unregister.
 *
 * Use this macro to construct bus specific macros for registering
 * drivers, and do not use it on its own.
 */
#define module_driver(__driver, __register, __unregister, ...) \
static int __init __driver##_init(void) \
{ \
        return __register(&(__driver) , ##__VA_ARGS__); \      //“##”是符号连接符，其作用是连接形参，当可变参数__VA_ARGS__为空时，移除##前多余的逗号
} \
module_init(__driver##_init); \
static void __exit __driver##_exit(void) \
{ \
        __unregister(&(__driver) , ##__VA_ARGS__); \
} \
module_exit(__driver##_exit);
~~~

通过对上述代码可知，实际该驱动的**初始化函数**为宏platform_driver_register，**出口函数**为宏platform_driver_unregister。

## 2、实际初始化函数platform_driver_register分析

platform_driver_register为宏，其是在函数__platform_driver_register的基础上进行了一层封装，减少了开发者的工作量和避免不必要的错误(赋错参数)。

~~~c
/*
 * platform_device.h
 * use a macro to avoid include chaining to get THIS_MODULE
 */
#define platform_driver_register(drv) \
	__platform_driver_register(drv, THIS_MODULE)
extern int __platform_driver_register(struct platform_driver *,
					struct module *);

/**
 * platform.c
 * __platform_driver_register - register a driver for platform-level devices
 * @drv: platform driver structure
 * @owner: owning module/driver
 */
int __platform_driver_register(struct platform_driver *drv,
				struct module *owner)
{
	drv->driver.owner = owner;         			 //设定module,驱动编写均赋 THIS_MODULE
	drv->driver.bus = &platform_bus_type;   	 //总线类型，这里为平台总线
	drv->driver.probe = platform_drv_probe; 	 //平台总线下自定义的探测函数
	drv->driver.remove = platform_drv_remove;	 //平台总线下自定义的移除函数
	drv->driver.shutdown = platform_drv_shutdown;//平台总线下自定义的关机函数

	return driver_register(&drv->driver);        //注册驱动
}
EXPORT_SYMBOL_GPL(__platform_driver_register);

/**
 * driver.c
 * driver_register - register driver with bus
 * @drv: driver to register
 *
 * We pass off most of the work to the bus_add_driver() call,
 * since most of the things we have to do deal with the bus
 * structures.
 */
int driver_register(struct device_driver *drv)
{
	int ret;
	struct device_driver *other; 

	if (!drv->bus->p) { //判断该总线是否存在
		pr_err("Driver '%s' was unable to register with bus_type '%s' because the bus was not initialized.\n",
			   drv->name, drv->bus->name);
		return -EINVAL;
	}
    
    //总线对象的probe、remove、shutdown接口函数指针一般未初始化，因此若初始化，则设备驱动中同名接口函数指针应与其保持一致，即指向相同的函数
	if ((drv->bus->probe && drv->probe) ||
	    (drv->bus->remove && drv->remove) ||
	    (drv->bus->shutdown && drv->shutdown))
		printk(KERN_WARNING "Driver '%s' needs updating - please use "
			"bus_type methods\n", drv->name);

	other = driver_find(drv->name, drv->bus);//该函数进行了校验，即比对当前的总线下是否存在名字和现在需要注册的设备驱动的名字相同的设备驱动
	if (other) {
		printk(KERN_ERR "Error: Driver '%s' is already registered, "
			"aborting...\n", drv->name);
		return -EBUSY;
	}

	ret = bus_add_driver(drv);	//添加驱动到总线中，并执行匹配函数，匹配成功则执行探测函数
	if (ret)
		return ret;
	ret = driver_add_groups(drv, drv->groups);//在kobject对应的sysfs目录下创建一组属性文件
	if (ret) {
		bus_remove_driver(drv);
		return ret;
	}
	kobject_uevent(&drv->p->kobj, KOBJ_ADD);//生成uevent事件

	return ret;
}
EXPORT_SYMBOL_GPL(driver_register);


/**
 * bus.c
 * bus_add_driver - Add a driver to the bus.
 * @drv: driver.
 */
int bus_add_driver(struct device_driver *drv)
{
	struct bus_type *bus;
	struct driver_private *priv; //driver_private封装了设备驱动与其余组件大部分的联系
	int error = 0;

	bus = bus_get(drv->bus); //进行校验，避免设备驱动bus字段未初始化
	if (!bus)
		return -EINVAL;

	pr_debug("bus: '%s': add driver %s\n", bus->name, drv->name);

	priv = kzalloc(sizeof(*priv), GFP_KERNEL); //分配内核存储空间
	if (!priv) {
		error = -ENOMEM;
		goto out_put_bus;
	}
	klist_init(&priv->klist_devices, NULL, NULL);//对驱动下的设备链表进行初始化
	priv->driver = drv;
	drv->p = priv;
	priv->kobj.kset = bus->p->drivers_kset;//设备驱动对应的kobject所属的kset
	error = kobject_init_and_add(&priv->kobj, &driver_ktype, NULL,
				     "%s", drv->name);//对kobject对象进行初始化，并加入对应的kobject层级关系中
	if (error)
		goto out_unregister;

	klist_add_tail(&priv->knode_bus, &bus->p->klist_drivers);//将knode_bus对象挂接到总线下的驱动链表下
	if (drv->bus->p->drivers_autoprobe) {
		if (driver_allows_async_probing(drv)) {
			pr_debug("bus: '%s': probing driver %s asynchronously\n",
				drv->bus->name, drv->name);
			async_schedule(driver_attach_async, drv); //异步调度绑定函数
		} else {
			error = driver_attach(drv); //尝试将驱动与设备绑定
			if (error)
				goto out_unregister;
		}
	}
	module_add_driver(drv->owner, drv);
    // module_add_driver的作用：
    // 1、校验module_kobject对象是否存在，存在则进入步骤2，否则返回；（module_kobject对象中的kobj字段代表的含义为/sys/module/下的mod_name目录，“mod_name”即编译驱动生成的模块名，如spi-gpio.ko，则mod_name = “spi-gpio”）
    // 2、创建符号链接，在driver_private对象的kobj字段在sysfs文件系统中对应的目录下创建名为“module”的符号链接指向module_kobject对象中的kobj字段在sysfs文件系统中对应的目录
    // 3、生成一个驱动名，生成成功则进入步骤4，否则返回；（驱动名为“<bus_name>:<driver_name>”，如spi总线下有个spi-gpio驱动，则生成的驱动名为“spi:spi-gpio”）
    // 4、在module_kobject对象中的kobj字段在sysfs文件系统中对应的目录下创建一个目录，目录名为"drivers"
    // 5、在创建的目录下创建符号链接，符号链接名称为生成的驱动名，链接指向driver_private对象的kobj字段在sysfs文件系统中对应的目录

	error = driver_create_file(drv, &driver_attr_uevent);
	if (error) {
		printk(KERN_ERR "%s: uevent attr (%s) failed\n",
			__func__, drv->name);
	}
	error = driver_add_groups(drv, bus->drv_groups);
	if (error) {
		/* How the hell do we get out of this pickle? Give up */
		printk(KERN_ERR "%s: driver_create_groups(%s) failed\n",
			__func__, drv->name);
	}

	if (!drv->suppress_bind_attrs) {
		error = add_bind_files(drv);
		if (error) {
			/* Ditto */
			printk(KERN_ERR "%s: add_bind_files(%s) failed\n",
				__func__, drv->name);
		}
	}

	return 0;

out_unregister:
	kobject_put(&priv->kobj);
	/* drv->p is freed in driver_release()  */
	drv->p = NULL;
out_put_bus:
	bus_put(bus);
	return error;
}
~~~

在初始