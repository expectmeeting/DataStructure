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