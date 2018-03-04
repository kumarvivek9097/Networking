#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
int main(int argc, char **argv)
{
	int clientfd;
	short int port_no;
	char msg[1000];
	char buff[1000];

	struct sockaddr_in servaddr;
	char *server_ip_address;
	if (argc != 5) {
		printf("Usage Format: ./client -a <IPAddress> -p <PortNumber>\n");
		printf("Sample Run: ./client -a 127.0.0.1 -p 2000\n");
		exit(1);
	}
	server_ip_address = (char *)argv[2];
	port_no = atoi(argv[4]);
	printf("Client will connect to the server at IP %s, port #%d\n", server_ip_address, port_no);

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket could not be created\n");
		printf("errno %i: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("Client socket created\n");
	errno = 0;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_no);
	if ((inet_pton(AF_INET, server_ip_address, &servaddr.sin_addr)) <= 0) {
		printf("inet_pton error for %s\n", server_ip_address);
		printf("errno %d: %s\n", errno, strerror(errno));
		exit(1);
	}
	errno = 0;
	if ((connect(clientfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
		printf("Connect error\n");
		printf("errno %d: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("Client socket connected\n"); while (1)
	{
		printf("\nEnter the equation (eg. 2+2) ");
		scanf("%s", msg);
		send(clientfd, msg, strlen(msg) + 1, 0);


		if (recv(clientfd, buff, sizeof(buff), 0 ) > 0) {
			printf("Server Response: %s\n", buff);

		}
	}
	return 0;
}
