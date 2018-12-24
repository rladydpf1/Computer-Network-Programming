#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	int message;
	socklen_t len;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	len = sizeof(message);
	getsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, IPPROTO_IP, IP_TOS, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, IPPROTO_IP, IP_TTL, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	getsockopt(sock, IPPROTO_TCP, TCP_MAXSEG, (void *)&message, &len);
	write(sock, (void*)&message, sizeof(message));
	
	
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
