#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_PORT "10086"

void print_flags(struct addrinfo* p){

}

void print_family(struct addrinfo* p){
    printf("address family ");
    switch(p->ai_family){
        case AF_INET:
            printf("inet4");
            break;
        case AF_INET6:
            printf("inet6");
            break;
        case AF_UNIX:
            printf("unix");
            break;
        case AF_UNSPEC:
            printf("unspecified");
            break;
        default:
            printf("unknow!");
    }

}

void print_protocol(struct addrinfo* p){
    printf("protocol ");
    switch(p->ai_protocol){
        case 0:
            printf("default protocol");
            break;
        case IPPROTO_TCP:
            printf("tcp");
            break;
        case IPPROTO_UDP:
            printf("udp");
            break;
        case IPPROTO_RAW:
            printf("raw");
            break;
        default:
            printf("unknown: %d", p->ai_protocol);
            break;
    }
}

int main(){
    //struct sockaddr_in
    struct addrinfo hint;
    memset(&hint, 0, sizeof(addrinfo));

    hint.ai_family = AF_INET;
    hint.ai_flags = AI_ALL;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_socktype = SOCK_STREAM;

    struct addrinfo *result;
    int err;

    /*
    getaddrinfo(const char* host, const char* service, ...)

    service param: can be port number or specific service name,like sshd
    */


    err = getaddrinfo("localhost", "ssh", &hint, &result);
    if(err != 0){
        printf("getaddrinfo failed, %s\n",gai_strerror(err));
        exit(1);
    }

    for(struct addrinfo* p=result; p != NULL; p = p->ai_next){
        print_family(p);
        print_protocol(p);

        if(p->ai_family == AF_INET){
            char buf[INET_ADDRSTRLEN];
            struct sockaddr_in* addr = (struct sockaddr_in*)p->ai_addr;
            const char* addr_str = inet_ntop(AF_INET, &addr->sin_addr, buf, INET_ADDRSTRLEN);
            printf("addr: %s\n", addr_str);
            int port = ntohs(addr->sin_port);
            printf("port: %d\n", port);

        }
    }

    exit(0);

}