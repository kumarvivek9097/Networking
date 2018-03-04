#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> // for pid_t, child process - parents process
#include <arpa/inet.h>

#define BUF_SIZE 256
#define CLADDR_LEN 100

int main(int argc, char const *argv[])
{
    struct sockaddr_in server_addr, client; //server and client addr
    int sockfd;  // socket desc.
    int newsockfd; //newsocket desc.
    int len; // lenght of data read and write
    int ret; 
    char buffer[BUF_SIZE];
    pid_t childpid; // child process
    char clientAddr[CLADDR_LEN]; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error creating socket!\n");
        exit(1);
    }
    printf("Socket created!\n");

    memset(&server_addr, 0, sizeof(server_addr)); //resetting memory of server_addr
    server_addr.sin_family = AF_INET; //internet addr family
    server_addr.sin_addr.s_addr = INADDR_ANY; // listen to all interfaces in the host 
    server_addr.sin_port = htons(atoi(argv[1])); //port number
    bzero(&server_addr.sin_zero, 8); 

    ret = bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("Error binding!\n");
        exit(1);
    }
    printf("Binding done...\n");

    printf("Waiting for a connection...\n");
    listen(sockfd, 5);

    while(1) // //infinite loop, end with ctr + z
    {   
        len = sizeof(client);
        newsockfd = accept(sockfd, (struct sockaddr *) &client, &len);
        if (newsockfd < 0)
        {
            printf("Error accepting connection!\n");
            exit(1);
        }
        printf("Connection accepted...\n");

        inet_ntop(AF_INET, &(client.sin_addr), clientAddr, CLADDR_LEN);
        if ((childpid = fork()) == 0)
        {   //creating a child process
            close(sockfd);

//stop listening for new connections by the main process.
//the child will continue to listen.
//the main process now handles the connected client.
            while(1)
            {
                memset(buffer, 0, BUF_SIZE);
                //ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &client, &len);
                ret = recv(newsockfd, buffer, BUF_SIZE, 0);
                if (ret < 0)
                {
                    printf("Error receiving data!\n");
                    exit(1);
                }
                printf("Received: %s\n", buffer);
                //ret = sendto(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &client, len);
                ret = send(newsockfd, buffer, BUF_SIZE, 0);
                if (ret < 0)
                {
                    printf("Error sending data!\n");
                    exit(1);
                }
                printf("Sent: %s\n", buffer);
            }
        }
        close(newsockfd);
    }
}