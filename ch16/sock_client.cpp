#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 10086

int connect_retry(int fd, struct sockaddr* addr, socklen_t len){
    int err;
    int MAX_SLEEP = 128;
    int nsleep =1;

    for(; nsleep < MAX_SLEEP; nsleep = nsleep<<1){
        err = connect(fd, addr, len);
        if(err == 0){
            return 0;
        }else{
            printf("can not connect to server\n");
            sleep(nsleep);
        }
    }

    return -1;
}

int main(){
    //socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("new socket failed\n");
        exit(1);
    }

    //connect
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

    connect_retry(sockfd, (sockaddr*)&address, sizeof(sockaddr_in));

    //send/recv
    char buf[] = "hello,world";

    //todo: test 1025 byte?
    int nsend = send(sockfd, buf, sizeof(buf), 0);
    printf("send %d bytes\n", nsend);

    shutdown(sockfd, SHUT_WR);

    char recvbuf[1024];
    int nrecv;
    while ((nrecv = recv(sockfd, recvbuf, sizeof(recvbuf), 0)) > 0)
    {
        buf[nrecv] =0;
        printf("client recv: %s\n", buf);
    }

    shutdown(sockfd, SHUT_RD);
    close(sockfd);

    exit(0);
}