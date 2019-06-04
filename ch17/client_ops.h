#ifndef _CLIENT_OPS_H
#define _CLIENT_OPS_H

#include <unistd.h>

typedef struct {
    int fd;
    uid_t uid;    
} client_t;

#define E_SUCCESS 0
#define E_EMPTY_CLIENT 1
#define E_NOT_FOUND 2

extern client_t* g_clients;
extern int g_numOfClients;
//todo: use vector for god sake!!!

void client_alloc();
int client_delete(int fd);
int client_add(int fd, uid_t uid);

#endif