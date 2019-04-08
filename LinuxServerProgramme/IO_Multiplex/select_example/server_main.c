#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/select.h>
#include<time.h>
#include<set>

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
	
	char *buf = (char *)calloc(1024, sizeof(char));
	
	std::set<int> fds;
	fds.insert(server_connect_fd);
	
	fd_set read_fds;
	fd_set read_fds_kp;
	FD_ZERO(&read_fds);
	
	FD_SET(server_connect_fd, &read_fds);
	read_fds_kp = read_fds;
	
	while(1){	
		int ret = select(*fds.rbegin() + 1,&read_fds, NULL, NULL, NULL);
		
	
		if(ret < 0){
			printf("select failed, error type is %s\n", strerror(errno));
			return -1;
		}
	
		for(int i = 0; i < (*fds.rbegin() + 1); ++i){
			if(i == server_connect_fd && FD_ISSET(i,&read_fds)){
				struct sockaddr_in client_addr;
				socklen_t client_addr_len;
				int server_comm_fd = accept(server_connect_fd, (struct sockaddr *)&client_addr, &client_addr_len);
				if(server_comm_fd<0){
					printf("listen failed, error type is %s\n", strerror(errno));
					return -1;
				}
				
				fds.insert(server_comm_fd);
				FD_SET(server_comm_fd, &read_fds_kp);
			}
			else if(FD_ISSET(i,&read_fds)){
				memset(buf, 0, 1024);
		
				int recv_len = recv(i, buf, 1024, 0);
		
				if(recv_len < 0){
					printf("failed to recv\n");
				}
				else if(recv_len == 0){
					printf("null message\n");
				}
				else{
					if(strcmp("bye",buf)==0){
						printf("close communication fd:%d\n",i);
						close(i);
						FD_CLR(i,&read_fds_kp);
						fds.erase(i);
					}
					else{
						printf("from fd:%d Recv %d >> %s\n", i, recv_len, buf);
						send(i, buf, recv_len, 0);
					}
				}
			}
		}
		
		FD_ZERO(&read_fds);
		read_fds = read_fds_kp;
	}
	free(buf);
	
	close(server_connect_fd);
	printf("close server\n");
	
	return 1;
}