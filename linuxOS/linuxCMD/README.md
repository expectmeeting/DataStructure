[TOC]

# linux命令行

## 1、用户管理

linux中用户信息保存在/etc/passwd中，用户群组信息/etc/group中

- 创建用户： useradd [options] \[username]
  - -e [有效期限]：账号有效期限
  - -f [缓冲天数]：账号过期后间隔缓冲天数后关闭账号
  - -g [群组]：指定用户所属的群组
  - -G [群组]：指定用户所属的附属群组
- 设置密码：passwd [options] \[username]
  - -d：删除密码，仅有系统管理者（root）才能使用
  - -l：锁住密码
  - -u：解开已上锁的帐号
- 删除用户：userdel [options] \[username]
  - -f：强制删除用户，即使用户已经登录
  - -r：删除用户的同时，删除与用户相关的所有文件
- 切换用户：su [options] \[username]

## 2、安装软件

### 包安装工具

- centos系：rpm [options] \[parameter]
  - -i [套件档]：安装指定的套件档
  - -e [套件档]：卸载指定的套件档
  - -a：查询所有套件
  - -f [文件]：查询拥有指定文件的套件
  - -h：套件安装时列出标记
  - -q：使用询问模式，当遇到任何问题时，rpm指令会先询问用户
  - -p [套件档]：查询指定的RPM套件档
  - -l：显示套件的文件列表
  - -v：显示指令执行过程
  - -vv：详细显示指令执行过程，便于排错
  - -U [套件档]：升级指定的套件档
  - --force：忽略可能覆盖的文件，强制安装
  - --nodeps：安装时忽略依赖关系
- debian系：dpkg [options] \[parameter]
  - -i：安装软件包；
  - -r：删除软件包；
  - -P：删除软件包的同时删除其配置文件
  - -S  [keyword]：根据关键字搜索对应的deb包
  - -L：显示于软件包关联的文件
  - -l：显示已安装软件包列表
  - --unpack：解开软件包
  - -c：显示软件包内文件列表
  - --confiugre：配置软件包

### 软件包管理工具

采用软件包管理工具来管理软件包都需要在配置文件中设置软件包集合库的位置，即网络库或本地库，yum配置文件路径为/etc/yum.repos.d/centos-7.repo，apt-get配置文件路径为/etc/apt/source.list

- centos系：yum \[parameter] [package]
  - install：安装rpm软件包
  - update：更新rpm软件包
  - check-update：检查是否有可用的更新rpm软件包
  - remove：删除指定的rpm软件包
  - list：显示软件包的信息
  - search：检查软件包的信息
  - info：显示指定的rpm软件包的描述信息和概要信息
  - clean：清理yum过期的缓存
  - shell：进入yum的shell提示符
  - resolvedep：显示rpm软件包的依赖关系
  - localinstall：安装本地的rpm软件包
  - localupdate：显示本地rpm软件包进行更新
  - deplist：显示rpm软件包的所有依赖关系
- debian系：apt-get [options] \[parameter]
  - install：安装
  - remove：卸载
  - upgrade：升级
  - update：更新配置
- debian系：apt-cache [options] \[parameter]
  - search：搜索软件包
  - show：显示指定软件包的信息
  - depends：显示软件包的依赖关系
  - rdepends：显示软件包的反向依赖关系，即被什么软件包依赖
  - policy：软件包的安装状态和版本信息

### 下载解压缩方式

- 下载: wget [options] \[parameter]
  - -b：后台下载
  - -c：断点重传
  - -O [filename]：下载的文件以filename重命名
- 压缩/解压：tar [options] \[parameter]
  - -z：以gzip指令处理备份文件
  - -c：创建备份文件
  - -x：从备份文件中还原文件
  - -v：显示操作过程
  - -f [备份文件]：指定备份文件
  - -p：保留原有的文件权限
  - -t：列出备份文件的内容
  - -C：设置解压路径
- 配置环境变量：
  - export 显示所有环境变量
  - export PATH=/home/uusama/mysql/bin:$PATH 在原有变量后临时追加，窗口关闭后失效
  - vim ~/.bashrc，在最后一行追加 export PATH=/home/uusama/mysql/bin:$PATH，永久生效

## 3、文件管理

- 切换目录：cd [directory]
- 查看目录：ls 
- 打印文件到命令行：cat [filename]
- 改变文件权限：chmod
- 改变文件所属用户：chown
- 改变文件所属组：chgrp
- 搜索文件：find
- 搜索内容：grep
- 比较文件内容：diff
- 部分显示：more/less

## 4、程序运行

- 命令行运行
  - 运行./filename
  - 退出ctrl+c
- 后台运行
  - 运行nohup command >out.file 2>&1 &
  - 退出ps -ef | grep 关键字| awk \`{print $2}` |xargs kill -9
- 服务方式运行
  - 设置开机启动systemctl enable xxx
  - 启动systemctl start xxx
  - 关闭systemctl stop xxx
  - 查询状态systemctl status xxx
    - active（running）：一次或多次持续处理的运行
    - active（exited）：成功完成一次性的配置
    - active（waiting）:运行中，等待一个事件
    - inactive：不运行
    - enabled：开机启动
    - disabled：开机不启动
    - static：开机不启动，但可以被另一个启用的服务激活
  - 禁止自动和手动启动systemctl mask xxx
  - 取消禁止systemctl unmask xxx
  - 查看已激活项目systemctl list-units -t service
  - 查看所有项目systemctl list-units -t service -a
  - 查看系统装备的项目systemctl list-unit-files
  - 查看项目依赖关系systemctl list-dependencies [unit] \[--reverse]

## 5、编译链接

- nm [options] \[files]

  列出.o .a .so中的符号信息，包括诸如符号的值，符号类型及符号名称等。所谓符号，通常指定义出的函数，全局变量等等nm

  - options:
    - -A 在每个符号信息的前面打印所在对象文件名称
    - -C 输出demangle过了的符号名称,适用于C++程序
    - -D 打印动态符号
    - -l 使用对象文件中的调试信息打印出所在源文件及行号，即gcc编译时使用过-g选项
    - -n 按照地址/符号值来排序
    - -u 打印出那些未定义的符号
  - 常见符号类型
    - A 该符号的值在今后的链接中将不再改变
    - B 该符号放在BSS段中，通常是那些未初始化的全局变量
    - D 该符号放在普通的数据段中，通常是那些已经初始化的全局变量
    - T 该符号放在文本段中
    - U 该符号未定义过，需要自其他对象文件中链接进来
    - W 未明确指定的弱链接符号；同链接的其他对象文件中有它的定义就用上，否则就用一个系统特别指定的默认值