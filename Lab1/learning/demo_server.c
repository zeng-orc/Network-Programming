#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

void* client_handler(void* arg){
    int* client_socket_fd = arg;
    if (send(*client_socket_fd, "Hello", sizeof("Hello"), 0) == -1)
        perror("send");
    close(*client_socket_fd);
    free(client_socket_fd);
    return NULL;
}

int main(){
    static const char * const SERVICE_PORT = "8080";
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE,
    };
    struct addrinfo* result = NULL;
    
    int rc = 0;
    if ((rc = getaddrinfo(NULL, SERVICE_PORT, &hints, &result)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    int sockfd = -1;
    for (struct addrinfo* p = result; p != NULL; p = p->ai_next){
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("server: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(result);

    if (listen(sockfd, 5) == -1){
        perror("listen");
        return 1;
    }

    while (1){
        struct sockaddr_storage their_addr;
        unsigned int sin_size = sizeof(their_addr);
        int* new_fd = calloc(1, sizeof(int));
        *new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (*new_fd == -1){
        perror("accept");
        continue;
        }

        pthread_attr_t tattr;
        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

        pthread_t tid;
        pthread_create(&tid, &tattr, client_handler, new_fd);
    }

    return 0;
}
