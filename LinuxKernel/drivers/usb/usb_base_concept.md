[TOC]

# USB基本概念

## 1、硬件接口

- 分为公头(plug)和母头(receptacle)

- 普通PC上USB接口叫type,引脚定义

  | 引脚 |  功能  | 颜色 |   备注   |
  | :--: | :----: | :--: | :------: |
  |  1   | V  Bus |  红  | 电源正5V |
  |  2   | Data-  |  白  |  数据-   |
  |  3   | Data+  |  绿  |  数据+   |
  |  4   |  GND   |  黑  |    地    |

- 功能机使用的mini USB接口定义

  | 引脚 |  功能  | 颜色 |                 备注                 |
  | :--: | :----: | :--: | :----------------------------------: |
  |  1   | V  Bus |  红  |               电源正5V               |
  |  2   | Data-  |  白  |                数据-                 |
  |  3   | Data+  |  绿  |                数据+                 |
  |  4   |   ID   |      | A型：与地相连<br />B型：不接地（空） |
  |  5   |  GND   |  黑  |                  地                  |

- 此外还有之前智能机使用的Micro USB和当前智能机使用的type-c

## 2、硬件通路

CPU-总线-USB主控制器-USB设备

## 3、USB主控制器

主控制器用于控制所有USB设备的通信，CPU在把任务交给主控制器后，由主控制器完成剩余的时间后再通知CPU。

- 主控制器遵照不同标准实现：
  - UHCI(Universal Host Controller Interface)：通用主机控制接口，USB1.0/1.1
  - OHCI(Open Host Controller Interface)：开放主机控制接口，USB1.0/1.1，相比UHCI智能程度更高
  - EHCI(Enhanced Host Controller Interface)：增强主机控制接口，USB2.0，兼容UHCI和OHCI标准
  - XHCI(eXtensible Host Controller Interface)：可扩展主机控制接口，USB3.0，兼容上述标准
- 不同USB标准的速率：
  - USB 1.0 低速（Low Speed） 1.5Mbps
  - USB 1.1 全速（Full Speed） 12Mbps
  - USB 2.0 高速（High Speed） 480Mbps
  - USB 3.0 超高速（Super Speed） 5Gbps

## 4、拓扑结构

usb设备通过星型拓扑与主控制器相连，每个usb host控制器都会自带一个usb hub ，被称为根Hub。这个根hub可以接子hub，每个hub上挂载usb设备。通过外接usb hub，可以插更多的usb设备。当usb设备插入到usb Hub或从上面拔出时，都会发出电信号通知系统。usb主控制器芯片拥有一个7bit的地址空间，意味着其可以提供128个地址，由于主控制器自动为设备初始化保留地址“0”，因此总共有127个地址可以供外部的USB设备使用，且子hub也将占用一个地址，且一个usb设备具有多个功能，则每个功能都会占用一个地址。

## 5、OTG技术

OTG（On-The-Go）技术主要实现在没有主机（Host）的情况下，从设备间的数据传送。这是因为USB设备分为Host（主设备）和Slave（从设备），只有当一台Host设备和一台Slave设备连接时才能实现数据的传输。而OTG设备既能做主机又能做设备。

## 6、USB热拔插之硬件实现

USB集线器的每个下游端口的D+和D-上，分别接了一个15KΩ的下拉电阻到地。这样，在集线器的端口悬空时，就被这两个下拉电阻拉到了低电平，而在USB设备端，在D+或D-上接了1.5KΩ上拉电阻，这样，当设备插入到集线器时，由1.5KΩ的上拉电阻和15KΩ的下拉电阻分压，结果技术将查分数据线中的一条拉高了，集线器检测到这个状态后，就会报告给USB主控制器（或者通过它上一层的集线器报告给USB主控制器），这样就检测到设备的插入了。USB高速设备显示被识别为全速设备，然后通过Host和device两者之间的确认，再切换到相应模式。