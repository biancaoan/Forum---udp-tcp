#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SIZE 100
#define PORT 9600


int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv;
    struct hostent *host;   
    char message[SIZE];        
    size_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    host = gethostbyname(argv[1]);
    if (host == NULL) {
        fprintf(stderr, "gethostbyname\n");
        close(sockfd);
        exit(1);
    }
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    memcpy(&serv.sin_addr, host->h_addr, host->h_length);
     
    printf("Enter a message:\n");

    memset(message, 0, sizeof(message));
    len = read(0, message, SIZE - 1);
    if (len < 0) {
        perror("read");
        close(sockfd);
        exit(1);
    }

    if (len > 20) len = 20;

    int n = sendto(sockfd, message, len, 0,(struct sockaddr *)&serv, sizeof(serv));
    if (n < 0) {
        perror("sendto");
        close(sockfd);
        exit(1);
    }

    printf("Message successfully sent to server!!\n");

    close(sockfd);

    return 0;
}