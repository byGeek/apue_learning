#include "unix_domain_socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/stat.h>


int server_listen(const char* name){
    int sockfd;
    struct sockaddr_un addr;
    int err;
    int len;

    if(strlen(name) >= sizeof(addr.sun_path)){
        return -1;
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("socket fail");
        return -2;
    }

    unlink(name);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, name);
    //len = (int)(&((struct sockaddr_un*)0)->sun_path) + strlen(name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path);

    err = bind(sockfd, (const sockaddr*)&addr, len);
    if(err <0){
        perror("bind failed");
        return -3;
    }

    err = listen(sockfd, 64);
    if(err <0){
        perror("listen failed");
        return -4;
    }

    return sockfd;
    
}

int server_accept(int listenfd, uid_t* puid){
    struct sockaddr_un addr;
    socklen_t len;

    /*
can not get addr, why??

need to initialize len with exact sockadd_un length to get the addr

ref: http://man7.org/linux/man-pages/man2/accept.2.html

The addrlen argument is a value-result argument: the caller must
       initialize it to contain the size (in bytes) of the structure pointed
       to by addr; on return it will contain the actual size of the peer
       address.
*/

    len = sizeof(addr);
    int clientfd = accept(listenfd, (struct sockaddr*)&addr, &len);
    if(clientfd <0){
        perror("accept failed");
        return -1;
    }

    char client_path[1024];
    int path_len = len - offsetof(struct sockaddr_un, sun_path);
    memcpy(client_path, addr.sun_path, path_len);
    client_path[path_len] = 0;

    struct stat buf;
    if(stat(addr.sun_path, &buf)){
        perror("stat fail");
        return -2;
    }


    if (S_ISSOCK(buf.st_mode) == 0)
    {
        perror("not a unix socket!");
        return -3;
    }

    //todo: more check, check time and file permission

    if(puid != NULL){
        *puid = buf.st_uid;
    }
    unlink(addr.sun_path);

    return clientfd;
}

int client_connect(const char* name){
    int clientfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(clientfd <0){
        perror("socket");
        return -1;
    }

    int err;
    struct sockaddr_un client_addr;
    struct sockaddr_un server_addr;

    memset(&client_addr, 0, sizeof(struct sockaddr_un));
    client_addr.sun_family = AF_UNIX;
    char buf[] = "/var/tmp/myclient.path";
    strcpy(client_addr.sun_path, buf);
    //strcpy(client_addr.sun_path, name);

    unlink(client_addr.sun_path);

    int len;
    len = offsetof(struct sockaddr_un, sun_path) + strlen(buf);

    err = bind(clientfd, (const struct sockaddr*)&client_addr, len);
    if(err <0){
        perror("bind fail");
        return -2;
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, name);  //server well-known name
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    err = connect(clientfd, (const sockaddr*)&server_addr, len);
    if(err <0){
        perror("connect failed");
        unlink(client_addr.sun_path);
        return -3;
    }

    return clientfd;


}
