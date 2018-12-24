#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void error_handling(char *message);

int main(int argc, char *argv[]){
	int fd;
	int check;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		error_handling("open() error!");
	char buf[100];
	int fp;
	fp = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0644);
	if (fp == -1)
		error_handling("open() error!");
	
	while (check = (read(fd, buf, sizeof(buf)))){
		if (check > 0) write(fp, buf, check);
		else if (check == -1) error_handling("write() error!");
		else break;
	}

	close(fd);
	close(fp);

	printf("Copy finished from %s to %s\n", argv[1], argv[2]);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

