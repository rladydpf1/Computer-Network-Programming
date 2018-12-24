#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]){
	char *addr, *str_arr, command[30];
	struct sockaddr_in addr_inet;
	while (1){
		printf("--------------------------------------------------\n");
		printf("Input Dotted-Decimal Address: ");
		scanf("%s", command);
		if (strcmp(command, "quit") ==  0) break;
		addr = command;
		printf("inet_aton : %s => ",addr);
		if (!inet_aton(addr, &addr_inet.sin_addr))
			printf("Conversion error");
		else
			printf("%lld\n",(long long) addr_inet.sin_addr.s_addr);

		printf("inet_ntoa : %lld => "
				,(long long) addr_inet.sin_addr.s_addr);
		str_arr = inet_ntoa(addr_inet.sin_addr);
		printf("%s\n", str_arr);
	}
	return 0;
}
