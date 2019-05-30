#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 10086
#define BACK_LOG 128

int main(){
    //socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("new socket failed\n");
        exit(1);
    }

    //bind
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", (void*)&address.sin_addr.s_addr);

    int err = bind(sockfd, (const sockaddr*)&address, sizeof(address));
    if(err != 0){
        printf("bind failed\n");
        close(sockfd);
        exit(1);
    }


    //listen

    err = listen(sockfd, 128);
    if(err != 0){
        printf("listen failed\n");
        close(sockfd);
        exit(1);
    }

    //accept
    printf("wait for connection...\n");
    int clientfd = accept(sockfd, NULL, NULL);
    printf("got a connection!\n");


    //fork another process to handle new connection?

    char buf[1024];
    int nrecv;
    int ntotol_recv =0;
    while((nrecv =recv(clientfd, buf, sizeof(buf) -1, 0)) >0){
        buf[nrecv] = 0;
        printf("server recved: %s\n", buf);
        ntotol_recv += nrecv;
    }

    printf("server prepare to send\n");

    snprintf(buf, sizeof(buf), "got %d bytes", ntotol_recv);

    send(clientfd, buf, sizeof(buf), 0);

    shutdown(clientfd, SHUT_RDWR);
    close(clientfd);

    close(sockfd);

}