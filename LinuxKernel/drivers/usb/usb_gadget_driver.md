[TOC]

# USB器件驱动

usb器件驱动主要工作在含有linux系统的嵌入式设备上，使得该设备成为一个usb从设备而存在。

## 1、层次结构

通常运行在USB设备端的子系统至少在内核中含有三层用于进行USB协议的处理。从下到上，分为：

- USB控制器驱动(USB device controller driver)

  该层位于软件架构最底层，也是唯一一层通过寄存器、fifos、dma、irq等与硬件通信的一层。在<linux/usb/gadget.h>中定义的API抽象了端点硬件。硬件通过端点对象暴露给软件，并通过回调与器件驱动交互，而端点对象负责接收IN/OUT缓冲区流。尽管普通的USB设备只有一个上游端口，因此只有一个驱动程序，但支持任意数量的器件驱动程序，只是同一时间只能使用一个。

  目录：/gadget/udc/

  涉及文件：gadget/udc-core.c和具体UDC控制器的实现文件(填充usb_gadget对象，对usb_gadget_ops和usb_ep_ops对象初始化，调用usb_add_gadget_udc函数将填充的usb_gadget对象用usb_udc对象进行管理，并加入udc_list链表中）

- 器件驱动（Gadget Driver）

  器件驱动程序的下边界通过对控制器驱动程序的调用来实现硬件无关的USB功能。由于硬件在功能和限制上有很大的差异，并且在空间非常大的嵌入式环境中使用，因此gadget驱动程序通常在编译时用特定控制器支持的端点进行配置。使用条件编译，器件驱动程序可以移植到多个不同的控制器。（最近的内核通过为许多面向批量的驱动程序自动配置端点，大大简化了支持新硬件的工作。）Gadget驱动程序的职责包括：

  - 处理setup请求（ep0协议响应），可能包括特定类的功能

  - 返回配置和字符串描述符

  - （re）设置配置和接口配置，包括启用和配置端点

  - 处理生命周期事件，例如管理与硬件的绑定、USB挂起/恢复、远程唤醒和从USB主机断开连接。

  - 管理所有当前已启用端点上的IN/OUT传输

- 功能层


## 2、重要结构体

- usb_composite_driver：为器件分配配置

  ~~~c
  struct usb_composite_driver {
  	const char				*name;//为了判别，对驱动标识
  	const struct usb_device_descriptor	*dev;//设备标识符，包括默认设备标识符
  	struct usb_gadget_strings		**strings;
  	enum usb_device_speed			max_speed;//器件支持的最大传输速率
  	unsigned		needs_serial:1;//设置为1则需要用户空间来提供序列号，没有提供会打印警告
  
  	int			(*bind)(struct usb_composite_dev *cdev);//（必选）用于分配在整个器件上共享的资源，如字符串id，并使用@usb_add_config（）为其添加配置。返回一个负的errno值可能会失败；成功初始化时应该返回零。
  	int			(*unbind)(struct usb_composite_dev *);
  
  	void			(*disconnect)(struct usb_composite_dev *);//可选的驱动程序断开连接的方法
  
  	/* global suspend hooks */
  	void			(*suspend)(struct usb_composite_dev *);
  	void			(*resume)(struct usb_composite_dev *);
  	struct usb_gadget_driver		gadget_driver;//控制此驱动程序的gadget驱动程序
  };
  ~~~

- usb_composite_dev：表示一个复合usb器件

  ~~~c
  struct usb_composite_dev {
  	struct usb_gadget		*gadget;//只读，器件中USB控制器
  	struct usb_request		*req;//用于控制响应，预分配buffer
  	struct usb_request		*os_desc_req;//用于OS描述符响应，预分配buffer
  
  	struct usb_configuration	*config;//当前有效的配置
  
  	/* OS String is a custom (yet popular) extension to the USB standard. */
  	u8				qw_sign[OS_STRING_QW_SIGN_LEN];
  	u8				b_vendor_code;
  	struct usb_configuration	*os_desc_config;
  	unsigned int			use_os_string:1;
  
  	/* private: */
  	/* internals */
  	unsigned int			suspended:1;
  	struct usb_device_descriptor	desc;
  	struct list_head		configs;
  	struct list_head		gstrings;
  	struct usb_composite_driver	*driver;
  	u8				next_string_id;
  	char				*def_manufacturer;
  
  	/* the gadget driver won't enable the data pullup
  	 * while the deactivation count is nonzero.
  	 */
  	unsigned			deactivations;
  
  	/* the composite driver won't complete the control transfer's
  	 * data/status stages till delayed_status is zero.
  	 */
  	int				delayed_status;
  
  	/* protects deactivations and delayed_status counts*/
  	spinlock_t			lock;
  
  	unsigned			setup_pending:1;
  	unsigned			os_desc_pending:1;
  };
  ~~~

- usb_gadget_driver：usb从设备的驱动程序

  ~~~c
  struct usb_gadget_driver {
  	char			*function;//描述器件功能的字符串
  	enum usb_device_speed	max_speed;//传输的最大速率
  	int			(*bind)(struct usb_gadget *gadget,//驱动程序的绑定回调
  					struct usb_gadget_driver *driver);//解除绑定
  	void			(*unbind)(struct usb_gadget *);
  	int			(*setup)(struct usb_gadget *,
  					const struct usb_ctrlrequest *);
  	void			(*disconnect)(struct usb_gadget *);
  	void			(*suspend)(struct usb_gadget *);
  	void			(*resume)(struct usb_gadget *);
  	void			(*reset)(struct usb_gadget *);
  
  	/* FIXME support safe rmmod */
  	struct device_driver	driver;
  
  	char			*udc_name;
  	struct list_head	pending;
  	unsigned                match_existing_only:1;
  };
  ~~~

- usb_gadget：表示一个usb从设备

  ~~~c
  struct usb_gadget {
  	struct work_struct		work;//用于sysfs_notify()的工作队列
  	struct usb_udc			*udc;
  	/* readonly to gadget driver */
  	const struct usb_gadget_ops	*ops;//用于访问硬件特定操作的函数指针
  	struct usb_ep			*ep0;//端点0，用于响应读取或写入驱动setup()请求
  	struct list_head		ep_list;	/* of usb_ep *///设备支持的其它端点列表
  	enum usb_device_speed		speed;//当前与usb主机的传输速率
  	enum usb_device_speed		max_speed;//UDC能处理的最大速率
  	enum usb_device_state		state;//当前状态（连接、休眠、配置等）
      //usb设备状态：
      //1、接入态（attached）：设备接入主机后，主机通过信号线上的电平变化来发现设备的接入
      //2、供电态（powered）：为设备供电，分为接入时的默认供电值，配置后的供电值
      //3、缺省态（default）：usb在被配置前，通过缺省地址0与主机进行通信
      //4、地址态（Address）：经过配置，USB设备复位后，按主机分配的唯一地址与主机通信，就叫地址态
  	//5、配置态（Configured）：通过各种标准USB请求命令来获取设备的各种信息，并对设备的某些信息进行改变或设置
  	//6、挂起态（Suspended）：USB总线处于空闲状态的话，该设备就要自动进入挂起状态，在进入挂起状态后，总的电流功耗不超过280UA
  	const char			*name;//表示控制器硬件类型，用于识别和配置
  	struct device			dev;
  	unsigned			out_epnum;//上次使用的out端点
  	unsigned			in_epnum;//上次使用的in端点
  	unsigned			mA;//上次设置的mA值
  	struct usb_otg_caps		*otg_caps;//器件的otg功能
  
  	unsigned			sg_supported:1;//能处理scatter/gather IO,则为1
  	unsigned			is_otg:1;//是否支持otg
  	unsigned			is_a_peripheral:1;
  	unsigned			b_hnp_enable:1;
  	unsigned			a_hnp_support:1;
  	unsigned			a_alt_hnp_support:1;
  	unsigned			hnp_polling_support:1;
  	unsigned			host_request_flag:1;
  	unsigned			quirk_ep_out_aligned_size:1;
  	unsigned			quirk_altset_not_supp:1;
  	unsigned			quirk_stall_not_supp:1;
  	unsigned			quirk_zlp_not_supp:1;
  	unsigned			quirk_avoids_skb_reserve:1;
  	unsigned			is_selfpowered:1;//是否是自供电设备
  	unsigned			deactivated:1;//是否激活
  	unsigned			connected:1;//是否与host连接，即是否能使能上拉电阻，触发host枚举
  };
  ~~~

- usb_gadget_ops：控制器硬件的其余API

  ~~~c
  struct usb_gadget_ops {
  	int	(*get_frame)(struct usb_gadget *);
  	int	(*wakeup)(struct usb_gadget *);
  	int	(*set_selfpowered) (struct usb_gadget *, int is_selfpowered);
  	int	(*vbus_session) (struct usb_gadget *, int is_active);
  	int	(*vbus_draw) (struct usb_gadget *, unsigned mA);
  	int	(*pullup) (struct usb_gadget *, int is_on);
  	int	(*ioctl)(struct usb_gadget *,
  				unsigned code, unsigned long param);
  	void	(*get_config_params)(struct usb_dcd_config_params *);
  	int	(*udc_start)(struct usb_gadget *,
  			struct usb_gadget_driver *);
  	int	(*udc_stop)(struct usb_gadget *);
  	struct usb_ep *(*match_ep)(struct usb_gadget *,
  			struct usb_endpoint_descriptor *,
  			struct usb_ss_ep_comp_descriptor *);
  };
  ~~~

- usb_udc：描述一个usb设备控制器

  ~~~c
  struct usb_udc {
  	struct usb_gadget_driver	*driver;
  	struct usb_gadget		*gadget;
  	struct device			dev;//实际控制器的子设备
  	struct list_head		list;//udc类驱动使用的链表
  	bool				vbus;//供电电源线的状态，对于不关心其状态的udc，该值总为true
  };
  ~~~

- usb_request：描述一个I/O请求

  ~~~c
  struct usb_request {
  	void			*buf;//用于存储数据的buffer,一些控制器只用PIO，或部分端点使用PIO
  	unsigned		length;//数据长度
  	dma_addr_t		dma;//dma地址
  
  	struct scatterlist	*sg;
  	unsigned		num_sgs;
  	unsigned		num_mapped_sgs;
  
  	unsigned		stream_id:16;
  	unsigned		no_interrupt:1;
  	unsigned		zero:1;
  	unsigned		short_not_ok:1;
  
  	void			(*complete)(struct usb_ep *ep,
  					struct usb_request *req);
  	void			*context;
  	struct list_head	list;//被器件驱动使用的链表
  
  	int			status;//返回完成结果，0->成功
  	unsigned		actual;//实际传输的数据长度
  };
  ~~~

- usb_ep：表示一个端点

  ~~~c
  struct usb_ep {
  	void			*driver_data;//端点私有数据，一般指向usb_composite_dev对象
  
  	const char		*name;//端点名称
  	const struct usb_ep_ops	*ops;
  	struct list_head	ep_list;//管理gadget设备建立的所有端点的链表
  	struct usb_ep_caps	caps;
  	bool			claimed;
  	bool			enabled;
  	unsigned		maxpacket:16;//该端点使用的最大包长度
  	unsigned		maxpacket_limit:16;
  	unsigned		max_streams:16;
  	unsigned		mult:2;
  	unsigned		maxburst:5;
  	u8			address;
  	const struct usb_endpoint_descriptor	*desc;
  	const struct usb_ss_ep_comp_descriptor	*comp_desc;
  };
  ~~~

- usb_ep_ops：指定部分usb控制器硬件api

  ~~~c
  struct usb_ep_ops {
  	int (*enable) (struct usb_ep *ep,
  		const struct usb_endpoint_descriptor *desc);
  	int (*disable) (struct usb_ep *ep);
  
  	struct usb_request *(*alloc_request) (struct usb_ep *ep,
  		gfp_t gfp_flags);
  	void (*free_request) (struct usb_ep *ep, struct usb_request *req);
  
  	int (*queue) (struct usb_ep *ep, struct usb_request *req,
  		gfp_t gfp_flags);
  	int (*dequeue) (struct usb_ep *ep, struct usb_request *req);
  
  	int (*set_halt) (struct usb_ep *ep, int value);
  	int (*set_wedge) (struct usb_ep *ep);
  
  	int (*fifo_status) (struct usb_ep *ep);
  	void (*fifo_flush) (struct usb_ep *ep);
  };
  ~~~
