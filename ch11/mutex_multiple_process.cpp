#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>


//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void gettimestr(char* buf, size_t len){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm* tp;

    tp = localtime(&tv.tv_sec);
    strftime(buf, len, "%F %r", tp);
}

int main(){
    pid_t pid;
    pthread_mutexattr_t mutex_attr;
    int err;

    err = pthread_mutexattr_init(&mutex_attr);
    if(err !=0){
        assert(0);
        printf("init mutex failed\n");
    }

    err = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    if(err !=0){
        assert(0);
        printf("set mutex shared between process failed\n");
    }

    pthread_mutex_t mutex;
    err = pthread_mutex_init(&mutex, &mutex_attr);
    assert(err == 0);

    pthread_mutexattr_destroy(&mutex_attr);
    /*mutex inherited with child process can access the mutex independently??*/

    if((pid = fork()) <0){
        assert(0);
    }else if(pid >0){
        err = pthread_mutex_lock(&mutex);
        if(err < 0){
            printf("parent lock failed\n");
            exit(1);
        }
        char buf[128];
        gettimestr(buf, 128);
        printf("%s: parent locked mutex\n", buf);
        sleep(3);

        pthread_mutex_unlock(&mutex);
        gettimestr(buf, 128);
        printf("%s: parent unlock mutex!\n", buf);
    }else{
        err = pthread_mutex_lock(&mutex);
        if(err < 0){
            printf("child lock failed\n");
            exit(1);
        }
        char buf[128];
        gettimestr(buf, 128);
        printf("%s: child locked mutex!\n", buf);
        sleep(3);
        pthread_mutex_unlock(&mutex);
        gettimestr(buf, 128);
        printf("%s: child unlock mutex!\n", buf);
    }

    exit(0);
}