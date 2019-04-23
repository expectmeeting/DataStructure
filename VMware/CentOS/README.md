## Centos中遇到的一些问题

## 1、固定IP

先查看本地网卡：

~~~
# ifconfig
~~~

查看第一个网卡名称，我的是**ens33**,然后打开网卡配置文件

~~~
# vim etc/sysconfig/network-scripts/ifcfg-ens33
~~~

修改**BOOTTPROTP**和**IPADDR**，IPADDR就是自己想设置的固定IP

~~~
BOOTTPROTP=STATIC
IPADDR=192.168.254.144
~~~

最后，重启网络

~~~
service network restart
~~~

