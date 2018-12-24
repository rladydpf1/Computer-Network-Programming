#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char file_name[30];
	int check, key;
	int fp;
	char buf[1024];
	
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");

	printf("Input file name: ");
	scanf("%s", file_name);
	write(sock, file_name, sizeof(file_name));

	check = (read(sock, buf, sizeof(buf)));
	if (check == 0)
       	{
		printf("File Not Found\n");
	}
	
	else
       	{
		fp = open(file_name, O_CREAT|O_WRONLY|O_TRUNC, 0644);
		write(fp, buf, check);
		while (check = (read(sock, buf, sizeof(buf)))) 
		{
			if (check > 0) write(fp, buf, check);
			else break;
		}
		printf("File %s received\n", file_name);
		close(fp);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
