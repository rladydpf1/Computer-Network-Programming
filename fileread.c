#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 30

int main() {
	int fd1, fd2, check;
	char buf1[BUF_SIZE] = {0}, buf2[BUF_SIZE] = {0};

	fd1 = open("data1.txt", O_RDONLY);
	fd2 = dup(fd1);
	
	while (1) {
		check = read(fd1, buf1, sizeof(buf1));
		if (check > 0) write(1, buf1, check);
		else break;
		
		check = read(fd2, buf2, sizeof(buf2));
		if (check > 0) write(1, buf2, check);
		else break;
	}
	printf("\n");
	close(fd1);
	close(fd2);
}
