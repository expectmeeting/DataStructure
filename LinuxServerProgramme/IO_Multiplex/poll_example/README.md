# 采用select的回声服务器

## 服务器

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<poll.h>
#include<time.h>
#include<set>

#define MAX_CONN 5

int main()
{
	int server_connect_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_connect_fd < 0){
		printf("creat socket failed，error type is %s\n", strerror(errno));
		return -1;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)9090);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int on = 1;
	if(setsockopt(server_connect_fd, cd, SO_REUSEADDR, &on, sizeof(on)) < 0){
		printf("setsockopt failed, error type is %s\n", strerror(errno));
		return -1;
	}
	
	
	if(bind(server_connect_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("bind failed, error type is %s\n", strerror(errno));
		return -1;
	}
		
	if(listen(server_connect_fd,2) < 0){
		printf("listen failed, error type is %s\n", strerror(errno));
		return -1;
	}
	
	char *buf = (char *)calloc(1024, sizeof(char));
	
	bool bCloseServer = false;
	
	std::set<int> fds;
	fds.insert(server_connect_fd);
	
	struct pollfd fd[MAX_CONN];
	
	fd[0].fd = server_connect_fd;
	fd[0].events = POLLIN;
	
	for(int i = 1;i < MAX_CONN; ++i){
		fd[i].fd = -1;
	}
	
	while(1){	
		int ret = poll(fd, fds.size(), -1);
		
	
		if(ret < 0){
			printf("select failed, error type is %s\n", strerror(errno));
			return -1;
		}
		else if(ret == 0){
			printf("overtime\n");
		}
		else{
			for(int i = 0; i < MAX_CONN; ++i){
				if(fd[i].fd == server_connect_fd && (POLLIN & fd[i].revents)){
					struct sockaddr_in client_addr;
					socklen_t client_addr_len;
					int server_comm_fd = accept(server_connect_fd, (struct sockaddr *)&client_addr, &client_addr_len);
					if(server_comm_fd<0){
						printf("listen failed, error type is %s\n", strerror(errno));
						return -1;
					}
					
					if(fds.size()>= MAX_CONN){
						printf("the number of communication is more than the MAX_CONN\n");
						close(server_comm_fd);
					}
					else{
						fds.insert(server_comm_fd);
						for(int i = 1;i < MAX_CONN; ++i){
							if(fd[i].fd == -1){
								fd[i].fd = server_comm_fd;
								fd[i].events = POLLIN;
								break;
							}
						}
					}	
				}
				else if(fd[i].fd != -1 && (POLLIN & fd[i].revents)){
					memset(buf, 0, 1024);
			
					int recv_len = recv(fd[i].fd, buf, 1024, 0);
			
					if(recv_len < 0){
						printf("failed to recv\n");
					}
					else if(recv_len == 0){
						printf("null message\n");
					}
					else{
						if(strcmp("bye",buf)==0 || strcmp("bye bye",buf)==0){
							printf("close communication fd:%d\n",fd[i].fd);
							close(fd[i].fd);
							fd[i].fd = -1;
							fd[i].events = 0;
							fds.erase(i);
							if(strcmp("bye bye",buf)==0){
								bCloseServer = ture;
							}
						}
						else{
							printf("from fd:%d Recv %d >> %s\n", fd[i].fd, recv_len, buf);
							send(fd[i].fd, buf, recv_len, 0);
						}
					}
				}
			}
		}
		if(bCloseServer){
			break;
		}
	}
	free(buf);
	
	close(server_connect_fd);
	printf("close server\n");
	
	return 1;
}
~~~

## 客户端

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
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0){
		printf("creat socket failed，error type is %s\n", strerror(errno));
		return -1;
	}
	
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons((unsigned short)9090);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//192.168.254.143 127.0.0.1
	
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
		scanf("%[^\n]", buf);
		getchar();
		
		int send_len = send(client_fd, buf, strlen(buf), 0);
		if(send_len < 0){
			printf("failed to send\n");
		}
		else if(send_len == 0){
			printf("null message\n");
		}
		else{
			printf("send %d Bytes\n", send_len);
			if(strcmp("bye",buf)==0){
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

## 使用

由于使用了STL中的set，因此采用g++编译

g++ -o sout server_main.c