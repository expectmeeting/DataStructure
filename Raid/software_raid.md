# 软raid管理命令

linux下软raid的管理工具为mdadm，其需要内核提供相应的md驱动和raid1等驱动支持，其对raid的管理是基于分区（partition），但这值得商榷，如raid级别为container。

命令的语法格式：

mdadm [mode] <raiddevice> [options] <component-devices>

**支持的模式：**

Assemble：装配一个以前定义的阵列 
Build：创建一个没有超级块的阵列 
Create：创建一个新的阵列，每个设备具有超级块 
Manage： 管理阵列(如添加和删除) 
Misc：允许单独对阵列中的某个设备进行操作(如停止阵列) 
Follow or Monitor:监控RAID的状态 
Grow：改变RAID的容量或阵列中的设备数目 

**相应的命令：**

~~~shell
#装配
-A ： --assemble：装配一个以前定义的阵列
	example:
		mdadm -A /dev/md127 /dev/sdb1 /dev/sdc1
-B ： --build：创建一个没有超级块的阵列(Build a legacy array without superblocks.)
#创建
-C ： 
	-l ：级别
	-n : 设备个数
	-a {yes|no}: 是否自动为其创建设备文件，默认设备文件为/dev/md*
	-c：CHUNK大小, 2^n，默认为64K
	-x : 指定空闲盘（热备磁盘）个数，空闲盘（热备磁盘）能在工作盘损坏后自动顶替
  example:
  	 mdadm -C /dev/md0 -l 1 -a yes -n 2 /dev/sdb1 /dev/sdc1
#管理
-f ：指定磁盘为损坏
-a : 添加磁盘
-r : 移除磁盘
   example：
       mdadm /dev/md0 -f /dev/sdb1
       mdadm /dev/md0 -r /dev/sdb1
#Misc     
-D ： 显示riad-device或component-devices的信息
-Q :  --query：查看一个device，判断它为一个 md device 或是 一个 md 阵列的一部分
-E ： --examine：打印 device 上的 md superblock 的内容
-R ： 运行部分装配的磁盘
--zero-superblock : 将md的spuerblock用0重写
--kill-subarray= ： 删除raid级别为container的下的阵列
--update-subarray= ：在容器中指明一个子阵列 
#监控
-F, --follow, --monitor：选择监控(Monitor)模式
#改变
-G, --grow：改变激活阵列的大小或形态
-I, --incremental：添加一个单独的设备到合适的阵列，并可能启动阵列
--auto-detect：请求内核启动任何自动检测到的阵列
-h, --help：帮助信息，用在以上选项后，则显示该选项信息
--help-options：显示更详细的帮助
-V, --version：打印mdadm的版本信息
-v, --verbose：显示细节
-b, --brief：较少的细节。用于 --detail 和 --examine 选项
-c, --config= ：指定配置文件，缺省为 /etc/mdadm.conf
-s, --scan：扫描配置文件或 /proc/mdstat以搜寻丢失的信息。配置文件/etc/mdadm.conf
~~~

