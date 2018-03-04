#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <math.h>

float stof(const char* s);
float calculator(char *calculation);

extern int errno;

int main(int argc, char **argv)
{
	int clientaddrlen, listenfd, connectfd, bytes_rcvd, listen_queue_size = 1;
	short int port_no;
	char readBuff[1000];
	struct sockaddr_in servaddr, clientaddr;
	pid_t  childpid;
	int status;
	char sendBuff[1025];

	if (argc != 3) {
		printf("Usage Format: ./server -p <PortNumber>\n");
		printf("Sample Run: ./server -p 2000\n");
		exit(1);
	}
	port_no = atoi(argv[argc - 1]);
	printf("Server running at port #%d\n", port_no);

	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Cannot create server socket! errno %i: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("Server socket created\n");
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port_no);
	errno = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (errno < 0) {
		printf("Server bind failure errno %i: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("Server socket is bound to port #%d\n", port_no);
	errno = listen(listenfd, listen_queue_size);
	if (errno < 0) {
		printf("Server listen failure errno %i: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("Server listening with a queue of size %d. \n", listen_queue_size);

	while (1) {
		clientaddrlen = sizeof(clientaddr);
		connectfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddrlen);
		if (connectfd < 0) {
			printf("Server accept failure errno %d: %s\n", errno, strerror(errno));
			exit(1);
		}
		printf("A connection received from a client. Creating a child to serve the client.\n");
		if ((childpid = fork()) == 0) {
			close(listenfd);
			printf("Child process serving the client.\n");
			if (recv(connectfd, readBuff, sizeof(readBuff), 0 ) > 0) {
				printf("Received message: %s\n", readBuff);
				float answer = calculator(readBuff);
				//printf("%f\n",answer );
				char msg[60];
				char chAns[1000] = "";
				sprintf(chAns, "%f", answer);
				//memcpy(chAns, answer, 30);
				strcat(msg, chAns);
				write(connectfd, msg, strlen(readBuff));
			}
			exit(1);
		}
		else if (childpid < 0) {
			printf("Failed to fork\n");
			printf("Fork error errno %d: %s\n", errno, strerror(errno));
		}
		else if (childpid != 0) {
			close(connectfd);
			childpid = wait(&status);
		}
	}
	return 0;
}

float calculator(char calculation[256])
{

	char *token;

	char *val1;
	char *val2;

	float  v1;
	float  v2;
	float  ans = -9999.0f;

	bool validCal = true;

	printf("****\n");

	char tempCalculation[256];

	strcpy(tempCalculation, calculation);

	printf("****\n");

	token = strtok(tempCalculation, "+-*//^");

	int c = 0;
	while (token != NULL)
	{
		c++;
		printf("%d\n", c);

		if (c == 1)
			val1 = token;

		if (c == 2)
			val2 = token;

		printf("%s\n", token);
		token = strtok(NULL, "+-*//^");

	}

	if (c == 2)
	{

		int i1;
		const int lenVal1 = strlen(val1);
		for (i1 = 0; i1 < lenVal1 ; i1++)
		{
			if (!isdigit(val1[i1]))
				break;
		}
		if (i1 != lenVal1)
			validCal = false;

		int i2;
		const int lenVal2 = strlen(val2) - 1;
		for (i2 = 0; i2 < lenVal2 ; i2++)
		{
			if (!isdigit(val2[i2]))
				break;
		}
		if (i2 != lenVal2)
			validCal = false;

// printf("operand1:%soperand2:%s\nlen1:%dlen2%d\n",val1,val2,lenVal1,lenVal2);

		if (validCal)
		{
			char operator = calculation[lenVal1];

			printf("Operator%c\n", operator);
			printf("Calculation:%s\n", calculation);

			v1 = stof(val1);
			v2 = stof(val2);


			printf("v1=%f\n", v1);
			printf("v2=%f\n", v2);



			if (operator == '+')
			{
				printf("Addition\n");
				ans = v1 + v2;
			}
			else if (operator == '-')
			{
				printf("Substraction\n");
				ans = v1 - v2;
			}
			else if (operator == '*')
			{
				printf("Multiplication\n");
				ans = v1 * v2;
			}
			else if (operator == '/')
			{
				printf("Devision\n");
				ans = v1 / v2;
			}
			else if (operator == '^')
			{
				printf("Power\n");

			}

			else
			{

			}

			printf("Answer:%f\n", ans);

		}
		else
		{
			printf("The values are not numeric!\n");

		}

		return ans;

	}
	else
	{
		printf("Two operands are required for calculation\n");
		validCal = false;

		return ans;

	}

};

float stof(const char* s) {
	float rez = 0, fact = 1;
	if (*s == '-') {
		s++;
		fact = -1;
	};


	int point_seen;
	for (point_seen = 0; *s; s++) {
		if (*s == '.') {
			point_seen = 1;
			continue;
		};
		int d = *s - '0';
		if (d >= 0 && d <= 9) {
			if (point_seen) fact /= 10.0f;
			rez = rez * 10.0f + (float)d;
		};
	};
	return rez * fact;
};