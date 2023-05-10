#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

int main(){
    static const char * const SERVICE_PORT = "8080";
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo* result = NULL;
    
    int rc = 0;
    if ((rc = getaddrinfo("localhost", SERVICE_PORT, &hints, &result)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    int sockfd = -1;
    for (struct addrinfo* p = result; p != NULL; p = p->ai_next){
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    static const int MAXDATASIZE = 100;
    char buf[MAXDATASIZE];
    int numbytes = 0;
    if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1){
        perror("recv");
        return 1;
    }
    
    buf[numbytes] = 0;

    printf("client:recv %s\n", buf);

    close(sockfd);
    return 0;
}
