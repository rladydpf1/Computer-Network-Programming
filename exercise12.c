#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char *argv[]) {
	struct timeval timeout;
	fd_set reads, temps;
	int fd0, fd1, result;
	int check;
	char buf[BUF_SIZE];
	fd0 = open("testfile1", O_RDONLY);
	fd1 = open("testfile2", O_RDONLY);
	if (fd0 == -1)
		error_handling("open() error!");
	if (fd1 == -1)
		error_handling("open() error!"); 
	int fd_max, i;

	FD_ZERO(&reads);
	FD_SET(fd0, &reads);
	FD_SET(fd1, &reads);
	if (fd0 > fd1) fd_max = fd0;
	else fd_max = fd1;
	printf("fd0= %d, fd1= %d\n", fd0, fd1);

	while(1) {
		temps = reads;
		timeout.tv_sec = 3;
		timeout.tv_usec = 3000;
		if ((result = select(fd_max + 1, &temps, 0, 0, &timeout)) == -1)
			break;
		if (result == 0) break;
		else if (result > 0) {
			for (i = 3; i <= fd_max; i++) {
				if (FD_ISSET(i, &temps)) {
					printf("[%d] ", i);
					while (check = (read(i, buf, 100))){
						if (check > 0) printf("%s", buf);
						else break;
					}
					FD_CLR(i, &reads);
					close(i);
					printf("\nFile close: %d\n\n", i);
				}
				
			}
		}
		
	}
}

void error_handling(char *buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
