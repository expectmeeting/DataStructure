# 下载内核源码

linus在git上有linux更新的内核主线代码，但其开发并不依赖git。官方的内核下载地址为：

https://www.kernel.org/

该地址中，公布了mainline主线内核，stable稳定版内核，longterm长期维护版内核等。推荐下载长期维护版本。

下载方法为：

~~~shell
#示例下载长期稳定版 4.19.29
wget https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.19.29.tar.xz
~~~

由于国外网站不稳定，也可从国内镜像网站下载：

~~~shell
#阿里云镜像
wget https://mirrors.aliyun.com/linux-kernel/v4.x/linux-4.19.29.tar.gz
#清华镜像
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v4.x/linux-4.19.29.tar.gz
~~~



