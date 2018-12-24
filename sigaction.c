#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void keycontrol(int sig) {
	if (sig == SIGINT) {
		printf("\nSIGINT signal occured.\n");
	}
}

int main(int argc, char *argv[]) {
	printf("This is pid %d\n", getpid());
	struct sigaction act;
	act.sa_handler = keycontrol;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, 0);
	while (1);
	return 0;
}
