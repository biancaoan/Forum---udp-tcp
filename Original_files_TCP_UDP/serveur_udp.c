#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>

#include <string.h>

#define PORT 9600

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv;
    struct sockaddr_in cAddr;
    socklen_t len;
    char buffer[100];
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    } 
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;       
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(PORT);
    
    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    }
    
    while(1){
        len = sizeof(cAddr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,(struct sockaddr*)&cAddr, &len);
        buffer[n] = '\0';
        write(1, buffer, n);
        write(1, "\n", 1);
    }
    close(sockfd);
    return 0;
}
