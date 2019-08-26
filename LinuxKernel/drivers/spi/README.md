# SPI驱动

SPI即Serial Perripheral Interface串行外围设备接口，其利用4根管脚(SCLK、MOSI、MISO和SS)用来控制以及数据传输。

- SCLK：串行时钟，用于master设备往slave设备传输时钟信号
- MOSI：主机输出从机输入
- MISO：主机输入从机输出
- SS：片选信息，用于 Master 设备片选 Slave 设备, 一般低电平表示选中

SPI有四种模式，由相位(CPHA)和极性()CPOL两两组合产生：

CPHA：

- 0表示从SCLK的第一个边沿采样

- 1表示从SCLK的第二个边沿采样

CPOL：

- 0表示SPI空闲时，SCLK处于低电平
- 1表示SPI空闲时，SCLK处于高电平

## GPIO模拟SPI

由于SPI只需四根管脚，因此采用4根可编程管脚GPIO即可模拟SPI。

GPIO基础知识：

1、GPIO属于CPU的一种硬件资源，因此占用GPIO管脚需要向内核进行申请。

~~~c
/*
 *@gpio ,表示GPIO对应的编号，平台代码中，已经将所有的GPIO进行统一编号(或查询CPU手册)
 *@label,自定义标签
 *@return， 负数表示该GPIO被占用
 */
int gpio_request(unsigned gpio,const char *label)//申请GPIO资源
~~~

2、常用的GPIO函数

~~~c
gpio_direction_output(unsigned gpio,int value)//配置GPIO为输出口，并输出value(0,1)
gpio_direction_input(unsigned gpio)//配置为输入口
gpio_set_value(unsigned gpio,int value);//将GPIO寄存器设置为0或1，该函数并不设置GPIO为输出，只是把相应的寄存器设置为0
gpio_get_value(unsigned gpio);//同理，设置为输入
gpio_free(unsigned gpio);//释放GPIO资源
~~~

