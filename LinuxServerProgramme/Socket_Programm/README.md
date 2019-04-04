# linux下网络编程

linux下的网络编程主要是掌握一系列套接字编程函数。

## TCP的socket编程

### Server_socket

- 1、创建套接字(**socket**)
- 2、将套接字绑定(**bind**)至本地IP地址和端口
- 3、将套接字设置为监听(**listen**)，等待连接请求
- 4、接受请求(**accept**)，返回一个用于通信的套接字
- 5、采用通信套接字接收(**recv**)或发送(**send**)数据
- 6、结束通信，关闭套接字(**close**)

### Client_socket

- 1、创建套接字(**socket**)
- 2、向服务器发送请求(**connect**)
- 3、连接建立后，向服务器发送(**send**)或接收(**recv**)数据
- 4、结束通信，关闭套接字(**close**)

## UDP的socket编程

### Server_socket

- 1、创建套接字(**socket**)
- 2、将套接字绑定(**bind**)至本地IP地址和端口
- 3、采用套接字接收(**recvfrom**)或发送(**sendto**)数据
- 4、结束通信，关闭套接字(**close**)

### Client_socket

- 1、创建套接字(**socket**)
- 2、向服务器发送(**sendto**)或接收(**recvfrom**)数据
- 4、结束通信，关闭套接字(**close**)

## 套接字函数

### 1、socket

头文件：

~~~c
#include<sys/types.h>
#include<sys/socket.h>
~~~

函数：

~~~c
int socket(int domain, int type, int protocol);
~~~

功能：socket()创建一个用于通信的文件描述符(file descriptor)

参数：

- domain 协议族：
  - AF_UNIX 本机通信
  - AF_INET IPV4
  - AF_INET6 IPV6
- type 套接字类型：
  - SOCK_STREAM 流式套接字，用于TCP通信
  - SOCK_DGRAM 数据包套接字，用于UDP通信
  - SOCK_RAW 表示原始套接字
- protocol 套接字所用协议，不指定协议则设置为 0

返回：

- -1  创建失败，并设置errno

### 2、 bind

头文件：

```c
#include<sys/types.h>
#include<sys/socket.h>
```

函数：

```c
int bind(int socketfd, struct sockaddr *my_addr, socklen_t addrlen);
```

功能：bind()为套接字socketfd指定地址my_addr，my的长度为addrlen

参数：

- socketfd 函数socket()创建的套接字：

- my_addr 是一个struct sockaddr类型的指针对象，一般传入struct sockaddr_in指针：

  ~~~c
  struct sockaddr_in serv_addr;
  serv_addr.sin_len = sizeof(serv_addr);
  serv_addr.sin_famliy = AF_INET;
  serv_addr.sin_port = htons(5000);//将端口号5000转换为网络序传入参数
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  ~~~

- addrlen 指针my_addr指向类型的长度

返回：

- -1  绑定失败，并设置errno
- 0   绑定成功

示例：

~~~c
if(bind(servfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
    printf("bind failed,error type is %s",errno);
}
~~~

### 3、listen

头文件：

```c
#include<sys/socket.h>
```

函数：

```c
int listen(int socketfd, int backlog);
```

功能：将套接字socketfd设置为监听状态，并为完全建立连接的队列设置一个长度限制

参数：

- socketfd 函数socket()创建的套接字：
- backlog 表示完全建立连接(即状态为ESTABLISHED)的队列，未完成连接(状态为SYN_RCVD)套接字队列的最大长度可用tcp_max_syn_backlog设置：

返回：

- -1  绑定失败，并设置errno
- 0   监听成功

### 4、accept

头文件：

```c
#include<sys/types.h>
#include<sys/socket.h>
```

函数：

```c
int accept(int socketfd, struct sockaddr *my_addr, socklen_t *addrlen);
```

功能：从未完成连接队列中取出第一个完成连接的请求，返回一个用于通信的套接字。

参数：

- socketfd 函数socket()创建并设置为监听状态的套接字：
- my_addr 一个struct sockaddr类型的指针对象，用连接实体的地址填充：
- addrlen 一个socklen_t类型的指针对象。

返回：

- -1  绑定失败，并设置errno

### 5、send

头文件：

```c
#include<sys/types.h>
#include<sys/socket.h>
```

函数：

```c
int send(int socketfd, const void *msg, size_t len, int flags);
```

功能：仅用于连接套接字发送消息。

参数：

- socketfd 函数accept()返回的用于通信的连接套接字：
- msg 一个const void类型的指针对象，指向待发送的数据：
- len 待发送数据的长度。
- flags 标志字，一般为0。

返回：

- -1  发生本地错误。

### 6、recv

头文件：

```c
#include<sys/types.h>
#include<sys/socket.h>
```

函数：

```c
int recv(int socketfd, void *buf, size_t len, int flags);
```

功能：仅用于从连接套接字的内核接收缓冲区中拷贝数据。

参数：

- socketfd 用于通信的连接套接字：
- buf 一个void类型的指针对象，指向一个用于存放数据的缓冲区：
- len 缓冲区长度。
- flags 标志字，一般为0。

返回：

- -1  接收失败，并设置errno。

### 7、connect

头文件：

```c
#include<sys/types.h>
#include<sys/socket.h>
```

函数：

```c
int connect(int socketfd, const struct sockaddr *addr, socklen_t addrlen);
```

功能：向addr所指向的地址发起连接。

参数：

- socketfd 用于通信的套接字：
- addr 一个const struct sockaddr类型的指针对象，指向一个保存服务器地址和端口的对象：
- addrlen 指针addr所指向对象的大小。

返回：

- -1  接收失败，并设置errno。

### 8、close

头文件：

```c
#include<unistd.h>
```

函数：

```c
int close(int fd);
```

功能：关闭文件描述符。

参数：

- fd 文件描述符；
- addr 一个const struct sockaddr类型的指针对象，指向一个保存服务器地址和端口的对象：
- addrlen 指针addr所指向对象的大小。

返回：

- -1  接收失败，并设置errno。

## 示例 - 回声服务器

### 客户端

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main()
{
    //创建套接字
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0){
		printf("creat socket failed，error type is %s\n", strerror(errno));
		return -1;
	}
    
	//指定服务器IP地址和端口
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)9090);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
    //发起连接
	if(connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("connect failed，error type is %s\n", strerror(errno));
		return -1;
		
		
 	}
	
	char *buf = (char *) calloc(1024, sizeof(char));
	char *buf_recv = (char *) calloc(1024, sizeof(char));
	
	while(1){
		memset(buf, 0, 1024);
		memset(buf_recv, 0, 1024);
		
		printf("Input >> ");
		scanf("%[^\n]", buf);//输入字符串，直至输入\n即回车为止
		getchar();//将回车\n从输入缓存中清除
		
		int send_len = send(client_fd, buf, strlen(buf), 0);//空格的ASCII值与'\0'不同
		if(send_len < 0){
			printf("failed to send\n");
		}
		else if(send_len == 0){
			printf("null message\n");
		}
		else{
			printf("send %d Bytes\n", send_len);
			if(strcmp("bye",buf)==0 || strcmp("bye bye",buf)==0){
				printf("close communication\n");
				break;
			}
		}
		
		int recv_len = recv(client_fd, buf_recv, 1024,0);
		if(recv_len > 0){
			printf("Recv %d >> %s\n", recv_len, buf_recv);
		}
	}
	free(buf);
	free(buf_recv);
	
	close(client_fd);
	printf("close client\n");
	
	return 1;
}
~~~

### 服务器端

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

int main()
{
    //创建套接字
	int server_connect_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_connect_fd < 0){
		printf("creat socket failed，error type is %s\n", strerror(errno));
		return -1;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)9090);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
    //将绑定的端口设为重用，避免关闭套接字后，端口因时间未超出time_wait而无法重新使用
	int on = 1;
	if(setsockopt(server_connect_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
		printf("setsockopt failed, error type is %s\n", strerror(errno));
		return -1;
	}
	
	//绑定IP地址和端口
	if(bind(server_connect_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("bind failed, error type is %s\n", strerror(errno));
		return -1;
	}
		
    //监听
	if(listen(server_connect_fd,2) < 0){
		printf("listen failed, error type is %s\n", strerror(errno));
		return -1;
	}
	
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	
	char *buf = (char *)calloc(1024, sizeof(char));
	
	while(1){
        //用于通信的连接套接字
		int server_comm_fd = accept(server_connect_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(server_comm_fd<0){
			printf("listen failed, error type is %s\n", strerror(errno));
			return -1;
		}
		
		while(1){
			memset(buf, 0, 1024);
		
			int recv_len = recv(server_comm_fd, buf, 1024, 0);
		
			if(recv_len < 0){
				printf("failed to recv\n");
			}
			else if(recv_len == 0){
				printf("null message\n");
			}
			else{
				if(strcmp("bye",buf)==0 || strcmp("bye bye",buf)==0){
					printf("close communication\n");
					close(server_comm_fd);
					break;
				}
				else{
					printf("Recv %d >> %s\n", recv_len, buf);
					send(server_comm_fd, buf, recv_len, 0);
				}
			}
		}
		if(strcmp("bye bye",buf)==0){
			break;
		}
	}
	free(buf);
	
	close(server_connect_fd);
	printf("close server\n");
	
	return 1;
}
~~~

