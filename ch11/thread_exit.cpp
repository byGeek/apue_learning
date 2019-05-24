#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thd_fn1(void*){
    printf("thread 1 starts");
    sleep(1);
    pthread_exit((void*)1);
}

void* thd_fn2(void*){
    printf("thread 2 start\n");
    sleep(1);
    return (void*)2;
}

int main(){
    pthread_t tid1;
    pthread_t tid2;
    int err;
    void* ret;

    err = pthread_create(&tid1, NULL, thd_fn1, NULL);
    if(err != 0){
        printf("pthread_create failed!\n");
        exit(0);
    }

    err = pthread_create(&tid2, NULL, thd_fn2, NULL);
    if(err != 0){
        printf("pthread_create failed!\n");
        exit(0);
    }

    err = pthread_join(tid1, &ret);
    if(err !=0){
        printf("pthread_join failed\n");
        exit(0);
    }

    printf("thd1 ret: %ld\n", (long)ret);

    err = pthread_join(tid2, &ret);
    if(err != 0){
        printf("pthread_join failed\n");
        exit(0);
    }
    printf("thd2 exit code: %ld\n", (long)ret);

}