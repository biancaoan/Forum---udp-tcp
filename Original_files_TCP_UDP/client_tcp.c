#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SIZE 200
#define PORT 9600

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv;
    struct hostent *host;
    char message[SIZE];
    ssize_t len;

    if (argc != 2) {
        fprintf(stderr, "usage: %s machine\n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    // Fork to create sender and receiver processes
    if (fork() == 0) {
        // Receiver 
        while (1) {
            memset(message, 0, sizeof(message));
            len = read(sockfd, message, SIZE - 1);
            if (len <= 0) break;
            write(1, message, (size_t)len);
        }
        close(sockfd);
        exit(0);
    } else {
        // Sender
        while (1) {
            memset(message, 0, sizeof(message));
            len = read(0, message, SIZE - 1);
            if (len <= 0) break;
            if (write(sockfd, message, (size_t)len) < 0) {
                perror("write");
                break;
            }
        }
        close(sockfd);
        exit(0);
    }

    return 0;
}
