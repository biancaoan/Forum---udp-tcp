#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 9600
#define SIZE 200

int main(int argc, char *argv[])
{
    int sockfd, sockC;
    struct sockaddr_in serv;
    struct sockaddr_in cAddr;
    socklen_t len;
    char buffer[SIZE];
    int n;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(1);
    }

    while (1) {
        len = sizeof(cAddr);
        sockC = accept(sockfd, (struct sockaddr *)&cAddr, &len);
        if (sockC < 0) {
            perror("accept");
            close(sockfd);
            exit(1);
        }

        /* One process per client */
        if (fork() == 0) {
            close(sockfd);

            /* Fork again: one reads from socket, one writes to socket */
            if (fork() == 0) {
                /* RECEIVER: socket -> stdout */
                while (1) {
                    memset(buffer, 0, sizeof(buffer));
                    n = read(sockC, buffer, SIZE - 1);
                    if (n <= 0) break;
                    write(1, buffer, n);
                }
                close(sockC);
                exit(0);
            } else {
                /* SENDER: stdin -> socket */
                while (1) {
                    memset(buffer, 0, sizeof(buffer));
                    n = read(0, buffer, SIZE - 1);
                    if (n <= 0) break;
                    if (write(sockC, buffer, n) < 0) {
                        perror("write");
                        break;
                    }
                }
                close(sockC);
                exit(0);
            }
        }

        /* Parent continues accepting new clients */
        close(sockC);
    }

    close(sockfd);
    return 0;
}
