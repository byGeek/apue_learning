#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/times.h>
#include <time.h>
#include <errno.h>
#include <string.h>

int  main(){
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    struct timespec tout;
    struct tm *tp;
    char buf[64];

    clock_gettime(CLOCK_REALTIME, &tout);
    tp =localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tp);
    printf("time: %s\n", buf);

    pthread_mutex_lock(&mutex);
    printf("mutex locked\n");

    tout.tv_sec += 10;

    int err = pthread_mutex_timedlock(&mutex, &tout);

    clock_gettime(CLOCK_REALTIME, &tout);
    tp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tp);
    printf("now time: %s\n", buf);

    if(err == 0){
        printf("mutex got locked again!\n");
    }else{
        printf("can not lock mutex twice: %s\n", strerror(err));
    }
}