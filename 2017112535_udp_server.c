#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 30
void error_handling(char *message);
void * rread(void *arg);
void * wwrite(void *arg);
sem_t sem_one, sem_two;
int serv_sock;
char message[BUF_SIZE];
char msg_bye[4] = "bye";
int str_len;
socklen_t clnt_adr_sz;
struct sockaddr_in serv_adr, clnt_adr;

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock==-1)
		error_handling("UDP socket creation error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	clnt_adr_sz = sizeof(clnt_adr);

	pthread_create(&id_t1, NULL, rread, NULL);
	pthread_create(&id_t2, NULL, wwrite, NULL);

	if (!pthread_join(id_t1, NULL) || !pthread_join(id_t2, NULL)) return 0;

	close(serv_sock);
	return 0;
}
void * rread(void *arg) {
	while (1) {
		sem_wait(&sem_two);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		message[str_len] = 0;
		printf("Message from client: %s\n", message);
		if(strcmp(message, "bye") == 0)
		{
			printf("Quit Chatting.\n");
			exit(1);
		}
		sem_post(&sem_one);
	}
	return NULL;
}

void * wwrite(void *arg) {
	while (1) {
		sem_wait(&sem_one);
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
		{
			sendto(serv_sock, msg_bye, strlen(msg_bye), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
			printf("Quit Chatting.\n");
			exit(1);
		}
		sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
		sem_post(&sem_two);
	}
	return NULL;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

