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
	int sock;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr, from_adr;
	if (argc != 3){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	while(1) {
		fputs("Input two numbers (-1 -1 to quit): ", stdout);
		scanf("%d %d", &N.num1, &N.num2);
		
		sendto(sock, (void *)&N, sizeof(N), 0,
			(struct sockaddr*)&serv_adr, sizeof(serv_adr));

		adr_sz = sizeof(from_adr);
		recvfrom(sock, (void *)&N, sizeof(N), 0, (struct sockaddr*)&serv_adr, &adr_sz);
		printf("%d, %d\n", N.num1, N.num2);		
		if (N.num1 == -1 || N.num2 == -1) {
			printf("Quit\n");
			break;
		}
		printf("Result is: %d\n", N.sum);

	}
	close(sock);
	return 0;
}


void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
