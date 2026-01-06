#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "users.h"

#define PORT 9600
#define SIZE 200

// Add a user to users.txt
static void addUsers(const char *username)
{
    FILE *f = fopen("users.txt", "a");
    if (f == NULL) return;
    fprintf(f, "%s\n", username);
    fclose(f);
}

// Remove a user from users.txt
static void removeUser(const char *username)
{
    FILE *f = fopen("users.txt", "r");
    FILE *tmp;
    char line[64];

    if (f == NULL) return;

    tmp = fopen("users_tmp.txt", "w");
    if (tmp == NULL) {
        fclose(f);
        return;
    }

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, username) != 0) {
            fprintf(tmp, "%s\n", line);
        }
    }

    fclose(f);
    fclose(tmp);

    remove("users.txt");
    rename("users_tmp.txt", "users.txt");
}

int main(int argc, char *argv[])
{
    int sockfd, sockC;
    struct sockaddr_in serv;
    struct sockaddr_in cAddr;
    socklen_t len;
    char buffer[SIZE];
    int n;
    char line[SIZE * 2];


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

        if (fork() == 0) {
            // Now each client that connects is a user
            // A process is forked for each client

            User user;
            FILE *forum;

            close(sockfd);

            // Ask client for username
            write(sockC, "Hello new user! Enter your username:\n", 38);

            memset(buffer, 0, sizeof(buffer));
            n = read(sockC, buffer, SIZE - 1);
            if (n <= 0) {
                close(sockC);
                exit(0);
            }
            buffer[n] = '\0';

            userInit(&user, buffer);

            addUsers(user.username);

            // Fork again to allow multiple sends/receives 
            if (fork() == 0) {
                // Receive - these are messages from users
                while (1) {
                memset(buffer, 0, sizeof(buffer));
                n = read(sockC, buffer, SIZE - 1);
                if (n <= 0) break;
                buffer[n] = '\0';

                userMessage(&user, buffer, line, (int)sizeof(line));

                forum = fopen("forum.txt", "a");
                if (forum != NULL) {
                    fputs(line, forum);
                    fclose(forum);
                }
            }
                //Remove user from users.txt once they are out of the forum
                removeUser(user.username);
                close(sockC);
                exit(0);
            
            } else {
                // Send - these are messages to users
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

        // Continue to allow new clients
        close(sockC);
    }

    close(sockfd);
    return 0;
}
