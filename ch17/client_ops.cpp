#include "client_ops.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NCLIENT 10

client_t* g_clients = NULL;
int g_numOfClients = 0;

void client_alloc(){
    if(g_clients == NULL){
        g_clients = (client_t*)malloc(sizeof(client_t)*NCLIENT);
    }else{
        g_clients = (client_t*)realloc(g_clients, (g_numOfClients + NCLIENT)*sizeof(client_t));
    }
    if(g_clients == NULL){
        printf("not enough memory,exit!\n");
        exit(1);
    }

    for(int i=g_numOfClients; i< g_numOfClients + NCLIENT; i++){
        g_clients[i].fd = -1;  //
    }

    g_numOfClients += NCLIENT;
}

int client_delete(int fd){
    if(g_clients == NULL){
        return E_EMPTY_CLIENT;
    }
    for(int i=0; i< g_numOfClients; ++i){
        if(g_clients[i].fd == fd){
            g_clients[i].fd = 1;
            return E_SUCCESS;
        }
    }

    return E_NOT_FOUND;
}

int client_add(int fd, uid_t uid){
    if(g_clients == NULL){
        client_alloc();
    }

again:
    for(int i=0; i<g_numOfClients; ++i){
        if(g_clients[i].fd == -1){
            g_clients[i].fd = fd;
            g_clients[i].uid = uid;
            return E_SUCCESS;
        }
    }

    client_alloc();
    goto again;
}