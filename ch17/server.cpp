#include "unix_domain_socket.h"
#include <sys/socket.h>
#include <stdio.h>

int main(){
    int listenfd, clientfd;

    listenfd = server_listen(SERVER_PATH);
    if(listenfd < 0){
        return -1;
    }

    uid_t uid;
    clientfd = server_accept(listenfd, &uid);
    if(clientfd < 0){
        return -2;
    }

    int num;
    char buf[1024];
    while((num = recv(clientfd, buf, sizeof(buf), 0)) >0){
        buf[num] = 0;
        printf("server recv: %s\n", buf);
    }

    if(num <0){
        perror("recv failed");
        return -3;
    }

    char sendbuf[] = "server ok!";
    num = send(clientfd, sendbuf, sizeof(sendbuf), 0);
    printf("send %d bytes\n", num);
    if (num < 0)
    {
        perror("send failed");
        return -4;
    }

    shutdown(clientfd, SHUT_RDWR);
    close(clientfd);
    close(listenfd);
}