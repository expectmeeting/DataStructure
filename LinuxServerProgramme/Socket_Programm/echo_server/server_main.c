#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

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
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//
	
	int on = 1;
	if(setsockopt(server_connect_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
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
	
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	
	char *buf = (char *)calloc(1024, sizeof(char));
	
	while(1){	
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