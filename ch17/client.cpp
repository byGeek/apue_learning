#include "unix_domain_socket.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

int main(){
    int clientfd;

    clientfd = client_connect(SERVER_PATH);
    if(clientfd <0){
        return -1;
    }

    printf("client connected\n");

    char sendbuf[] = "helloworld";
    char recvbuf[1024];

    int num = send(clientfd, sendbuf, sizeof(sendbuf), 0);
    if(num<0){
        perror("send fail");
        goto fail;
    }

    shutdown(clientfd, SHUT_WR);

    while((num=recv(clientfd, recvbuf, 1024, 0))>0){
        printf("client recv: %d bytes\n", num);
        recvbuf[num] = 0;
        printf("client recv: %s\n", recvbuf);
    }

    if(num <0){
        perror("recv fail");
        goto fail;
    }

    close(clientfd);
    exit(0);

fail:
    close(clientfd);
    exit(1);

}