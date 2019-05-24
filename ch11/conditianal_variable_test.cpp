#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t mutex;
static pthread_cond_t cond;

void* thd_fn1(void* arg){
    printf("in thd_fn1\n");
    sleep(5);
    pthread_cond_signal(&cond);
    printf("signal conditional variable\n");
}

int main(){
    pthread_t pid;

    int err;

    err = pthread_create(&pid, NULL, thd_fn1, NULL);
    if(err != 0){
        printf("pthread_create failed\n");
        exit(-1);
    }

    printf("in main\n");

    //mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&mutex, NULL);
    err = pthread_cond_init(&cond, NULL);
    if(err != 0){
        printf("pthread_cond_init failed\n");
        exit(-1);
    }

    pthread_mutex_lock(&mutex);
    printf("wait for signal\n");

    pthread_cond_wait(&cond, &mutex);

    pthread_mutex_unlock(&mutex);

    printf("done!");

    getchar();
}