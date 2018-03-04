#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUF_SIZE 256

int main(int argc, char**argv) 
{
	struct sockaddr_in server_addr, client; 
	int sockfd, ret;
	char buffer[BUF_SIZE];
	struct hostent * server;
	char * serverAddr;

	if (argc < 2) 
	{
		printf("usage: port number missing!\n");
		exit(1);
	}

	serverAddr = argv[1];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		printf("Error creating socket!\n");
		exit(1);
	}
	printf("Socket created...\n");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(serverAddr);
	server_addr.sin_port = htons(atoi(argv[2]));

	ret = connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (ret < 0) 
	{
		printf("Error connecting to the server!\n");
		exit(1);
	}
	printf("Connected to the server...\n");

	memset(buffer, 0, BUF_SIZE);
	

	while (1) 
	{
		printf("Send: ");
		fgets(buffer, BUF_SIZE, stdin);
		//ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
		ret = send(sockfd, buffer, BUF_SIZE, 0);
		if (ret < 0) 
		{
			printf("Error sending data!\n\t-%s", buffer);
		}
		//ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
		ret = recv(sockfd, buffer, BUF_SIZE, 0);
		if (ret < 0) 
		{
			printf("Error receiving data!\n");
		} 
		else 
		{
			printf("Received: ");
			fputs(buffer, stdout);
			
		}
	}

	return 0;
}