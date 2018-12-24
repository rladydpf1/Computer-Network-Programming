#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct number {
	int num1;
	int num2;
	int sum;
}; 
struct number N;

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int str_len;
	socklen_t clnt_adr_sz;

	struct sockaddr_in serv_adr, clnt_adr;
	if (argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	while(1){
		clnt_adr_sz = sizeof(clnt_adr);

		recvfrom(serv_sock, (void *)&N, sizeof(N), 0,
				(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (N.num1 == -1 || N.num2 == -1) {
			sendto(serv_sock, (void *)&N, sizeof(N), 0,
				(struct sockaddr*)&clnt_adr, clnt_adr_sz);
			printf("Quit (%d, %d)\n", N.num1, N.num2);
			break;
		}
		
		N.sum = N.num1 + N.num2;
		sendto(serv_sock, (void *)&N, sizeof(N), 0,
				(struct sockaddr*)&clnt_adr, sizeof(clnt_adr));	
		printf("%d + %d = %d\n", N.num1, N.num2, N.sum);	
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
