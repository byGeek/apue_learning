#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thd_fn(void* arg){
    printf("in thread. pid=%lu, tid=%lu\n", (unsigned long)getpid(), (unsigned long)pthread_self());
    return (void*)1
}

int main(){
    pthread_t tdi;

    if(pthread_create(&tdi, NULL, thd_fn, NULL) != 0){
        printf("pthread_create failed!\n");
        _exit(0);
    }

    printf("pid=%lu, tid=%lu\n", (unsigned long)getpid(), (unsigned long)tdi);
    sleep(1);
    exit(0);
}d