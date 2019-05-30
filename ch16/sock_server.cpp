#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 10086
#define BACK_LOG 128

static int quit_flag = 0;

void handle_client(int fd);

void sig_quit_handler(int sig){
    printf("got quit message, server quiting...");
    quit_flag = 1;
    //todo: send or recv last message then quit
}

int main(){

    struct sigaction action;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    action.sa_handler = sig_quit_handler;
    if(sigaction(SIGQUIT, &action, NULL) <0){
        printf("sigaction failed\n");
        exit(1);
    }

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

    while(!quit_flag){
        //accept
        printf("wait for connection...\n");
        int clientfd = accept(sockfd, NULL, NULL);
        printf("got a connection!\n");

        pid_t pid;
        if((pid = fork()) <0){
            printf("fork failed\n");
            exit(0);
        }else if(pid >0){
            close(clientfd);  //parent
            continue;
        }else{
            //child
            handle_client(clientfd);
            printf("child process finish\n");
            exit(0);
        }
    }

    close(sockfd);
    printf("server exit!\n");
}

void handle_client(int clientfd){
    char buf[1024];
    int nrecv;
    int ntotol_recv = 0;
    while ((nrecv = recv(clientfd, buf, sizeof(buf) - 1, 0)) > 0)
    {
        buf[nrecv] = 0;
        printf("server recved: %s\n", buf);
        ntotol_recv += nrecv;
    }

    printf("server prepare to send\n");

    snprintf(buf, sizeof(buf), "got %d bytes", ntotol_recv);

    send(clientfd, buf, sizeof(buf), 0);
    shutdown(clientfd, SHUT_RDWR);
    close(clientfd);
}